/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "otbSarImageMetadataInterface.h"
#include "otbSentinel1ImageMetadataInterface.h"

#include "otbMacro.h"
#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"
#include "otbXMLMetadataSupplier.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "ossim/ossimTimeUtilities.h"
#pragma GCC diagnostic pop
#else
#include "ossim/ossimTimeUtilities.h"
#endif
#include "itksys/SystemTools.hxx"

// useful constants
#include <otbMath.h>
#include <iomanip>

namespace otb
{

Sentinel1ImageMetadataInterface::Sentinel1ImageMetadataInterface()
{
}

bool Sentinel1ImageMetadataInterface::CanRead() const
{
  const std::string sensorID = GetSensorID();

  return sensorID.find("SENTINEL-1") != std::string::npos;
}

bool Sentinel1ImageMetadataInterface::HasCalibrationLookupDataFlag() const
{
  itkExceptionMacro("HasCalibrationLookupDataFlag(mds) not yet implemented in Sentinel1ImageMetadataInterface"); // TODO
}

const Sentinel1ImageMetadataInterface::LookupDataPointerType Sentinel1ImageMetadataInterface::CreateCalibrationLookupData(const short type) const
{
  bool sigmaLut = false;
  bool betaLut  = false;
  bool gammaLut = false;
  bool dnLut    = false;

  switch (type)
  {
  case SarCalibrationLookupData::BETA:
  {
    otbMsgDevMacro("betaNought");
    betaLut = true;
  }
  break;

  case SarCalibrationLookupData::GAMMA:
  {
    otbMsgDevMacro("gamma");
    gammaLut = true;
  }
  break;

  case SarCalibrationLookupData::DN:
  {
    otbMsgDevMacro("dn");
    dnLut = true;
  }
  break;

  case SarCalibrationLookupData::SIGMA:
  default:
    otbMsgDevMacro("sigmaNought");
    sigmaLut = true;
    break;
  }

  const ImageKeywordlistType imageKeywordlist = this->GetImageKeywordlist();

  // const double firstLineTime = Utils::LexicalCast<double>(imageKeywordlist.GetMetadataByKey("calibration.startTime"), "calibration.startTime(double)");

  // const double lastLineTime = Utils::LexicalCast<double>(imageKeywordlist.GetMetadataByKey("calibration.stopTime"), "calibration.stopTime(double)");
  using namespace ossimplugins::time;
  const ModifiedJulianDate firstLineTime = toModifiedJulianDate(imageKeywordlist.GetMetadataByKey("calibration.startTime"));
  const ModifiedJulianDate lastLineTime  = toModifiedJulianDate(imageKeywordlist.GetMetadataByKey("calibration.stopTime"));
  otbMsgDevMacro(<< "calibration.startTime: " << std::setprecision(16) << firstLineTime);
  otbMsgDevMacro(<< "calibration.stopTime : " << std::setprecision(16) << lastLineTime);

  // const std::string supportDataPrefix = "support_data."; //make && use GetBandPrefix(subSwath, polarisation)

  const int numOfLines = Utils::LexicalCast<int>(imageKeywordlist.GetMetadataByKey("number_lines"), "number_lines(int)");
  otbMsgDevMacro(<< "numOfLines   : " << numOfLines);

  const int count = Utils::LexicalCast<int>(imageKeywordlist.GetMetadataByKey("calibration.count"), "calibration.count");

  std::vector<Sentinel1CalibrationStruct> calibrationVectorList(count);
  double                                  lastMJD = 0;

  for (int i = 0; i < count; i++)
  {
    Sentinel1CalibrationStruct calibrationVector;

    std::stringstream prefix;
    prefix << "calibration.calibrationVector[" << i << "].";
    const std::string sPrefix = prefix.str();

    calibrationVector.line = Utils::LexicalCast<int>(imageKeywordlist.GetMetadataByKey(sPrefix + "line"), sPrefix + "line");

    // TODO: don't manipulate doubles, but ModifiedJulianDate for a better type
    // safety
    const std::string sAzimuth = imageKeywordlist.GetMetadataByKey(sPrefix + "azimuthTime");
    calibrationVector.timeMJD  = toModifiedJulianDate(sAzimuth).as_day_frac();
    calibrationVector.deltaMJD = calibrationVector.timeMJD - lastMJD;
    lastMJD                    = calibrationVector.timeMJD;

    otbMsgDevMacro(<< sPrefix << "line   : " << calibrationVector.line << " ;\t" << sPrefix << "timeMJD: " << std::setprecision(16) << calibrationVector.timeMJD
                   << " (" << sAzimuth << ")");

    Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey(sPrefix + "pixel"), calibrationVector.pixels, sPrefix + "pixel");

    // prepare deltaPixels vector
    int prev_pixels = 0;
    calibrationVector.deltaPixels.resize(calibrationVector.pixels.size());
    for (std::size_t p = 0, N = calibrationVector.pixels.size(); p != N; ++p)
    {
      calibrationVector.deltaPixels[p] = (calibrationVector.pixels[p] - prev_pixels);
      prev_pixels                      = calibrationVector.pixels[p];
    }

    if (sigmaLut)
    {
      Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey(sPrefix + "sigmaNought"), calibrationVector.vect, sPrefix + "sigmaNought");
    }

    if (betaLut)
    {
      Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey(sPrefix + "betaNought"), calibrationVector.vect, sPrefix + "betaNought");
    }

    if (gammaLut)
    {
      Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey(sPrefix + "gamma"), calibrationVector.vect, sPrefix + "gamma");
    }

    if (dnLut)
    {
      Utils::ConvertStringToVector(imageKeywordlist.GetMetadataByKey(sPrefix + "dn"), calibrationVector.vect, sPrefix + "dn");
    }

    calibrationVectorList[i] = calibrationVector;
  }

  Sentinel1CalibrationLookupData::Pointer sarLut = Sentinel1CalibrationLookupData::New();
  sarLut->InitParameters(type, firstLineTime.as_day_frac(), lastLineTime.as_day_frac(), numOfLines, count, calibrationVectorList);
  return static_cast<SarImageMetadataInterface::LookupDataPointerType> (sarLut);
}

void Sentinel1ImageMetadataInterface::ParseDateTime(const char* key, std::vector<int>& dateFields) const
{
  if (dateFields.size() < 1)
  {
    // parse from keyword list
    if (!this->CanRead())
    {
      itkExceptionMacro(<< "Invalid Metadata, not a valid product");
    }

    const ImageKeywordlistType imageKeywordlist = this->GetImageKeywordlist();
    if (!imageKeywordlist.HasKey(key))
    {
      itkExceptionMacro(<< "no key named " << key);
    }

    const std::string date_time_str = imageKeywordlist.GetMetadataByKey(key);
    Utils::ConvertStringToVector(date_time_str, dateFields, key, "T:-.");
  }
}

int Sentinel1ImageMetadataInterface::GetYear() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if (m_AcquisitionDateFields.size() > 0)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[0], "support_data.image_date:year(int)");
  }
  else
  {
    itkExceptionMacro(<< "Invalid year");
  }
  return value;
}

int Sentinel1ImageMetadataInterface::GetMonth() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if (m_AcquisitionDateFields.size() > 1)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[1], "support_data.image_date:month(int)");
  }
  else
  {
    itkExceptionMacro(<< "Invalid month");
  }
  return value;
}

int Sentinel1ImageMetadataInterface::GetDay() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if (m_AcquisitionDateFields.size() > 2)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[2], "support_data.image_date:day(int)");
  }
  else
  {
    itkExceptionMacro(<< "Invalid day");
  }
  return value;
}

int Sentinel1ImageMetadataInterface::GetHour() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if (m_AcquisitionDateFields.size() > 3)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[3], "support_data.image_date:hour(int)");
  }
  else
  {
    itkExceptionMacro(<< "Invalid hour");
  }
  return value;
}

int Sentinel1ImageMetadataInterface::GetMinute() const
{
  int value = 0;
  ParseDateTime("support_data.image_date", m_AcquisitionDateFields);
  if (m_AcquisitionDateFields.size() > 4)
  {
    value = Utils::LexicalCast<int>(m_AcquisitionDateFields[4], "support_data.image_date:minute(int)");
  }
  else
  {
    itkExceptionMacro(<< "Invalid minute");
  }
  return value;
}

int Sentinel1ImageMetadataInterface::GetProductionYear() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if (m_ProductionDateFields.size() > 0)
  {
    value = Utils::LexicalCast<int>(m_ProductionDateFields[0], "support_data.date:year(int)");
  }
  else
  {
    itkExceptionMacro(<< "Invalid production year");
  }
  return value;
}

int Sentinel1ImageMetadataInterface::GetProductionMonth() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if (m_ProductionDateFields.size() > 1)
  {
    value = Utils::LexicalCast<int>(m_ProductionDateFields[1], "support_data.date:month(int)");
  }
  else
  {
    itkExceptionMacro(<< "Invalid production month");
  }
  return value;
}

int Sentinel1ImageMetadataInterface::GetProductionDay() const
{
  int value = 0;
  ParseDateTime("support_data.date", m_ProductionDateFields);
  if (m_ProductionDateFields.size() > 2)
  {
    value = Utils::LexicalCast<int>(m_ProductionDateFields[2], "support_data.date:day(int)");
  }
  else
  {
    itkExceptionMacro(<< "Invalid production day");
  }
  return value;
}

double Sentinel1ImageMetadataInterface::GetPRF() const
{
  double                     value            = 0;
  const ImageKeywordlistType imageKeywordlist = this->GetImageKeywordlist();
  if (!imageKeywordlist.HasKey("support_data.pulse_repetition_frequency"))
  {
    return value;
  }

  value = Utils::LexicalCast<double>(imageKeywordlist.GetMetadataByKey("support_data.pulse_repetition_frequency"),
                                     "support_data.pulse_repetition_frequency(double)");

  return value;
}


Sentinel1ImageMetadataInterface::UIntVectorType Sentinel1ImageMetadataInterface::GetDefaultDisplay() const
{
  UIntVectorType rgb(3);
  rgb[0] = 0;
  rgb[1] = 0;
  rgb[2] = 0;
  return rgb;
}

double Sentinel1ImageMetadataInterface::GetRSF() const
{
  return 0;
}

double Sentinel1ImageMetadataInterface::GetRadarFrequency() const
{
  return 0;
}

double Sentinel1ImageMetadataInterface::GetCenterIncidenceAngle() const
{
  return 0;
}

std::vector<AzimuthFmRate> Sentinel1ImageMetadataInterface::GetAzimuthFmRate(const XMLMetadataSupplier &xmlMS) const
{
  std::vector<AzimuthFmRate> azimuthFmRateVector;
  // Number of entries in the vector
  int listCount = xmlMS.GetAs<int>("product.generalAnnotation.azimuthFmRateList.count");
  // This streams wild hold the iteration number
  std::ostringstream oss;
  for (int listId = 1 ; listId <= listCount ; ++listId)
  {
    oss.str("");
    oss << listId;
    // Base path to the data, that depends on the iteration number
    std::string path_root = "product.generalAnnotation.azimuthFmRateList.azimuthFmRate_" + oss.str();
    AzimuthFmRate afr;
    std::istringstream(xmlMS.GetAs<std::string>(path_root + ".azimuthTime")) >> afr.azimuthTime;
    afr.t0 = xmlMS.GetAs<double>(path_root + ".t0");
    afr.azimuthFmRatePolynomial = xmlMS.GetAsVector<double>(path_root + ".azimuthFmRatePolynomial",
    		' ', xmlMS.GetAs<int>(path_root + ".azimuthFmRatePolynomial.count"));
    azimuthFmRateVector.push_back(afr);
  }
  return azimuthFmRateVector;
}

std::vector<DopplerCentroid> Sentinel1ImageMetadataInterface::GetDopplerCentroid(const XMLMetadataSupplier &xmlMS) const
{
  std::vector<DopplerCentroid> dopplerCentroidVector;
  // Number of entries in the vector
  int listCount = xmlMS.GetAs<int>("product.dopplerCentroid.dcEstimateList.count");
  // This streams wild hold the iteration number
  std::ostringstream oss;
  for (int listId = 1 ; listId <= listCount ; ++listId)
  {
    oss.str("");
    oss << listId;
    // Base path to the data, that depends on the iteration number
    std::string path_root = "product.dopplerCentroid.dcEstimateList.dcEstimate_" + oss.str();
    DopplerCentroid dopplerCent;
    std::istringstream(xmlMS.GetAs<std::string>(path_root + ".azimuthTime")) >> dopplerCent.azimuthTime;
    dopplerCent.t0 = xmlMS.GetAs<double>(path_root + ".t0");
    dopplerCent.dopCoef = xmlMS.GetAsVector<double>(path_root + ".dataDcPolynomial",
						    ' ', xmlMS.GetAs<int>(path_root + ".dataDcPolynomial.count"));
    dopplerCent.geoDopCoef = xmlMS.GetAsVector<double>(path_root + ".geometryDcPolynomial",
						       ' ', xmlMS.GetAs<int>(path_root + ".geometryDcPolynomial.count"));
    dopplerCentroidVector.push_back(dopplerCent);
  }
  return dopplerCentroidVector;
}

std::vector<Orbit> Sentinel1ImageMetadataInterface::GetOrbits(const XMLMetadataSupplier &xmlMS) const
{
  std::vector<Orbit> orbitVector;
  // Number of entries in the vector
  int listCount = xmlMS.GetAs<int>("product.generalAnnotation.orbitList.count");
  // This streams wild hold the iteration number
  std::ostringstream oss;
  for (int listId = 1 ; listId <= listCount ; ++listId)
  {
    oss.str("");
    oss << listId;
    // Base path to the data, that depends on the iteration number
    std::string path_root = "product.generalAnnotation.orbitList.orbit_" + oss.str();
    Orbit orbit;

    auto dateStr = xmlMS.GetAs<std::string>(path_root + ".time");

    std::stringstream ss;
    auto facet = new boost::posix_time::time_input_facet("%Y-%m-%dT%H:%M:%S%F");
    ss.imbue(std::locale(std::locale(), facet));
    ss << xmlMS.GetAs<std::string>(path_root + ".time");
    ss >> orbit.time;

    orbit.position[0] = xmlMS.GetAs<double>(path_root + ".position.x");
    orbit.position[1] = xmlMS.GetAs<double>(path_root + ".position.y");
    orbit.position[2] = xmlMS.GetAs<double>(path_root + ".position.z");
    orbit.velocity[0] = xmlMS.GetAs<double>(path_root + ".velocity.x");
    orbit.velocity[1] = xmlMS.GetAs<double>(path_root + ".velocity.y");
    orbit.velocity[2] = xmlMS.GetAs<double>(path_root + ".velocity.z");
    orbitVector.push_back(orbit);
  }
  return orbitVector;
}

std::vector<CalibrationVector> Sentinel1ImageMetadataInterface::GetCalibrationVector(const XMLMetadataSupplier &xmlMS) const
{
  std::vector<CalibrationVector> calibrationVector;
  // Number of entries in the vector
  int listCount = xmlMS.GetAs<int>("calibration.calibrationVectorList.count");
  // This streams wild hold the iteration number
  std::ostringstream oss;
  for (int listId = 1 ; listId <= listCount ; ++listId)
  {
    oss.str("");
    oss << listId;
    // Base path to the data, that depends on the iteration number
    std::string path_root = "calibration.calibrationVectorList.calibrationVector_" + oss.str();

    CalibrationVector calVect;
    std::istringstream(xmlMS.GetAs<std::string>(path_root + ".azimuthTime")) >> calVect.azimuthTime;
    calVect.line = xmlMS.GetAs<int>(path_root + ".line");

    // Same axe for all LUTs
    MetaData::LUTAxis ax1;
    ax1.Size = xmlMS.GetAs<int>(path_root + ".pixel.count");
    ax1.Values = xmlMS.GetAsVector<double>(path_root + ".pixel", ' ', ax1.Size);

    MetaData::LUT1D sigmaNoughtLut;
    sigmaNoughtLut.Axis[0] = ax1;
    sigmaNoughtLut.Array = xmlMS.GetAsVector<double>(path_root + ".sigmaNought",
     		' ', xmlMS.GetAs<int>(path_root + ".sigmaNought.count"));
    calVect.sigmaNought = sigmaNoughtLut;

    MetaData::LUT1D betaNoughtLut;
    betaNoughtLut.Axis[0] = ax1;
    betaNoughtLut.Array = xmlMS.GetAsVector<double>(path_root + ".betaNought",
     		' ', xmlMS.GetAs<int>(path_root + ".betaNought.count"));
    calVect.betaNought = betaNoughtLut;

    MetaData::LUT1D gammaLut;
    gammaLut.Axis[0] = ax1;
    gammaLut.Array = xmlMS.GetAsVector<double>(path_root + ".gamma",
     		' ', xmlMS.GetAs<int>(path_root + ".gamma.count"));
    calVect.gamma = gammaLut;

    MetaData::LUT1D dnLut;
    dnLut.Axis[0] = ax1;
    dnLut.Array = xmlMS.GetAsVector<double>(path_root + ".dn",
     		' ', xmlMS.GetAs<int>(path_root + ".dn.count"));
    calVect.dn = dnLut;

    calibrationVector.push_back(calVect);
  }
  return calibrationVector;
}

std::vector<SARNoise> Sentinel1ImageMetadataInterface::GetNoiseVector(const XMLMetadataSupplier &xmlMS) const
{
  std::vector<SARNoise> noiseVector;
  // Number of entries in the vector
  int listCount = xmlMS.GetAs<int>("noise.noiseVectorList.count");
  // This streams wild hold the iteration number
  std::ostringstream oss;
  for (int listId = 1 ; listId <= listCount ; ++listId)
  {
    oss.str("");
    oss << listId;
    // Base path to the data, that depends on the iteration number
    std::string path_root = "noise.noiseVectorList.noiseVector_" + oss.str();
    SARNoise noiseVect;
    std::istringstream(xmlMS.GetAs<std::string>(path_root + ".azimuthTime")) >> noiseVect.azimuthTime;
    MetaData::LUT1D noiseLut;
    MetaData::LUTAxis ax1;
    ax1.Size = xmlMS.GetAs<int>(path_root + ".pixel.count");
    ax1.Values = xmlMS.GetAsVector<double>(path_root + ".pixel", ' ', ax1.Size);
    noiseLut.Axis[0] = ax1;
    noiseLut.Array = xmlMS.GetAsVector<double>(path_root + ".noiseLut",
    		' ', xmlMS.GetAs<int>(path_root + ".noiseLut.count"));
    noiseVect.noiseLut = noiseLut;
    noiseVector.push_back(noiseVect);
  }
  return noiseVector;
}

std::vector<BurstRecord> Sentinel1ImageMetadataInterface::GetBurstRecords(const XMLMetadataSupplier &xmlMS) const
{
  std::vector<BurstRecord> burstRecords;

  // number of burst records
  int listCount = xmlMS.GetAs<int>(0, "product.swathTiming.burstList.count");

  std::stringstream ss;
  auto facet = new boost::posix_time::time_input_facet("%Y-%m-%dT%H:%M:%S%F");
  ss.imbue(std::locale(std::locale(), facet));

  if (!listCount)
  {
    BurstRecord record;

    ss << xmlMS.GetAs<std::string>("product.imageAnnotation.imageInformation.productFirstLineUtcTime");
    ss >> record.azimuthStartTime;

    ss << xmlMS.GetAs<std::string>("product.imageAnnotation.imageInformation.productLastLineUtcTime");
    ss >> record.azimuthStopTime;

    record.startLine = 0;
    record.endLine = xmlMS.GetAs<int>("product.imageAnnotation.imageInformation.numberOfLines") - 1;
    record.startSample = 0;
    record.endSample = xmlMS.GetAs<int>("product.imageAnnotation.imageInformation.numberOfSamples") - 1;

    record.azimuthAnxTime = 0.;

    burstRecords.push_back(record);
  }
  else
  {
    // TODO
    otbGenericExceptionMacro(MissingMetadataException,
      <<"Burst record parsing not implemented yet");
  }

  return burstRecords;
}

std::unordered_map<std::string, GCPTime> Sentinel1ImageMetadataInterface::GetGCPTimes(const XMLMetadataSupplier &xmlMS,
                                                                                          const Projection::GCPParam & gcps) const
{
  std::unordered_map<std::string, GCPTime> gcpTimes;
  
  std::stringstream ss;
  auto facet = new boost::posix_time::time_input_facet("%Y-%m-%dT%H:%M:%S%F");
  ss.imbue(std::locale(std::locale(), facet));

  for (const auto & gcp : gcps.GCPs)
  {
    const std::string prefix = "product.geolocationGrid.geolocationGridPointList.geolocationGridPoint_" + gcp.m_Id + ".";
    GCPTime time;
    ss << xmlMS.GetAs<std::string>(prefix + "azimuthTime");
    ss >> time.azimuthTime;
    time.slantRangeTime = xmlMS.GetAs<double>(prefix + "slantRangeTime"); 
    gcpTimes[gcp.m_Id] = time;
  }

  return gcpTimes;
}

double Sentinel1ImageMetadataInterface::getBandTerrainHeight(const XMLMetadataSupplier &xmlMS) const
{
  double heightSum = 0.0;
  // Number of entries in the vector
  int listCount = xmlMS.GetAs<int>("product.generalAnnotation.terrainHeightList.count");
  // This streams wild hold the iteration number
  std::ostringstream oss;
  for (int listId = 1 ; listId <= listCount ; ++listId)
  {
    oss.str("");
    oss << listId;
    // Base path to the data, that depends on the iteration number
    std::string path_root = "product.generalAnnotation.terrainHeightList.terrainHeight_" + oss.str();
    heightSum += xmlMS.GetAs<double>(path_root + ".value");
  }
  return heightSum / (double)listCount;
}

void Sentinel1ImageMetadataInterface::ParseGdal(ImageMetadata & imd)
{
  // Metadata read by GDAL
  Fetch(MDTime::AcquisitionStartTime, imd, "ACQUISITION_START_TIME");
  Fetch(MDTime::AcquisitionStopTime, imd, "ACQUISITION_STOP_TIME");
  Fetch(MDStr::BeamMode, imd, "BEAM_MODE");
  Fetch(MDStr::BeamSwath, imd, "BEAM_SWATH");
  Fetch("FACILITY_IDENTIFIER", imd, "FACILITY_IDENTIFIER");
  Fetch(MDNum::LineSpacing, imd, "LINE_SPACING");
  Fetch(MDStr::Mission, imd, "MISSION_ID");
  Fetch(MDStr::Mode, imd, "MODE");
  Fetch(MDStr::OrbitDirection, imd, "ORBIT_DIRECTION");
  Fetch(MDNum::OrbitNumber, imd, "ORBIT_NUMBER");
  Fetch(MDNum::PixelSpacing, imd, "PIXEL_SPACING");
  Fetch(MDStr::ProductType, imd, "PRODUCT_TYPE");
  Fetch(MDStr::Instrument, imd, "SATELLITE_IDENTIFIER");
  Fetch(MDStr::SensorID, imd, "SENSOR_IDENTIFIER");
  Fetch(MDStr::Swath, imd, "SWATH");

  // Manifest file
  std::string ManifestFilePath = m_MetadataSupplierInterface->GetResourceFile(std::string("manifest\\.safe"));
  if (!ManifestFilePath.empty())
  {
    XMLMetadataSupplier ManifestMS(ManifestFilePath);
    imd.Add(MDTime::ProductionDate,
    		ManifestMS.GetFirstAs<MetaData::Time>("xfdu:XFDU.metadataSection.metadataObject_#.metadataWrap.xmlData.safe:processing.start"));
    imd.Add(MDTime::AcquisitionDate,
    		ManifestMS.GetFirstAs<MetaData::Time>("xfdu:XFDU.metadataSection.metadataObject_#.metadataWrap.xmlData.safe:acquisitionPeriod.safe:startTime"));
  }

  assert(m_MetadataSupplierInterface->GetNbBands() == imd.Bands.size());
  // Band metadata
  for (int bandId = 0 ; bandId < m_MetadataSupplierInterface->GetNbBands() ; ++bandId)
  {
    SARParam sarParam;
    Fetch(MDStr::Polarization, imd, "POLARISATION", bandId);
    std::string swath = Fetch(MDStr::Swath, imd, "SWATH", bandId);

    // Annotation file
    std::string AnnotationFilePath = m_MetadataSupplierInterface->GetResourceFile(std::string("annotation[/\\\\]s1[ab].*-")
                                                          + itksys::SystemTools::LowerCase(swath)
                                                          + std::string("-.*\\.xml"));
    if (AnnotationFilePath.empty())
      otbGenericExceptionMacro(MissingMetadataException,<<"Missing Annotation file for band '"<<swath<<"'");
    XMLMetadataSupplier AnnotationMS(AnnotationFilePath);

    sarParam.azimuthFmRates = this->GetAzimuthFmRate(AnnotationMS);
    sarParam.dopplerCentroids = this->GetDopplerCentroid(AnnotationMS);
    sarParam.orbits = this->GetOrbits(AnnotationMS);

    sarParam.burstRecords = this->GetBurstRecords(AnnotationMS);
    sarParam.azimuthTimeInterval = boost::posix_time::precise_duration(
      AnnotationMS.GetAs<double>("product.imageAnnotation.imageInformation.azimuthTimeInterval")
      * 1e6);
    sarParam.nearRangeTime = AnnotationMS.GetAs<double>("product.imageAnnotation.imageInformation.slantRangeTime");
    sarParam.rangeSamplingRate = AnnotationMS.GetAs<double>("product.generalAnnotation.productInformation.rangeSamplingRate");

    sarParam.gcpTimes = this->GetGCPTimes(AnnotationMS, m_Imd.GetGCPParam());

    m_Imd.Add(MDNum::NumberOfLines, AnnotationMS.GetAs<int>("product.imageAnnotation.imageInformation.numberOfLines"));
    m_Imd.Add(MDNum::NumberOfColumns, AnnotationMS.GetAs<int>("product.imageAnnotation.imageInformation.numberOfSamples"));
    m_Imd.Add(MDNum::AverageSceneHeight, this->getBandTerrainHeight(AnnotationFilePath));
    m_Imd.Add(MDNum::RadarFrequency, AnnotationMS.GetAs<double>("product.generalAnnotation.productInformation.radarFrequency"));
    m_Imd.Add(MDNum::PRF, AnnotationMS.GetAs<double>("product.imageAnnotation.imageInformation.azimuthFrequency"));
    m_Imd.Add(MDNum::CenterIncidenceAngle, AnnotationMS.GetAs<double>("product.imageAnnotation.imageInformation.incidenceAngleMidSwath"));

    // Calibration file
    std::string CalibrationFilePath = itksys::SystemTools::GetFilenamePath(AnnotationFilePath)
                                      + "/calibration/calibration-"
                                      + itksys::SystemTools::GetFilenameName(AnnotationFilePath);
    if (CalibrationFilePath.empty())
          otbGenericExceptionMacro(MissingMetadataException,<<"Missing Calibration file for band '"<<swath<<"'");
    XMLMetadataSupplier CalibrationMS(CalibrationFilePath);
    imd.Bands[bandId].Add(MDNum::CalScale, CalibrationMS.GetAs<double>("calibration.calibrationInformation.absoluteCalibrationConstant"));
    sarParam.calibrationVectors = this->GetCalibrationVector(CalibrationMS);
    std::istringstream(CalibrationMS.GetAs<std::string>("calibration.adsHeader.startTime")) >> sarParam.calibrationStartTime;
    std::istringstream(CalibrationMS.GetAs<std::string>("calibration.adsHeader.stopTime")) >> sarParam.calibrationStopTime;

    // Noise file
    std::string NoiseFilePath = itksys::SystemTools::GetFilenamePath(AnnotationFilePath)
                                            + "/calibration/noise-"
                                            + itksys::SystemTools::GetFilenameName(AnnotationFilePath);
    if (NoiseFilePath.empty())
          otbGenericExceptionMacro(MissingMetadataException,<<"Missing Noise file for band '"<<swath<<"'");
    XMLMetadataSupplier NoiseMS(NoiseFilePath);
    sarParam.noiseVector = this->GetNoiseVector(NoiseMS);
    
    imd.Bands[bandId].Add(MDGeom::SAR, sarParam);
  }
  SARCalib sarCalib;
  sarCalib.calibrationLookupFlag = HasCalibrationLookupDataFlag();
  LoadRadiometricCalibrationData(sarCalib);
  imd.Add(MDGeom::SARCalib, sarCalib);
}

void Sentinel1ImageMetadataInterface::ParseGeom(ImageMetadata & imd)
{
  Fetch(MDTime::AcquisitionStartTime, imd, "support_data.first_line_time");
  Fetch(MDTime::AcquisitionStopTime, imd, "support_data.last_line_time");
  Fetch(MDNum::LineSpacing, imd, "support_data.azimuth_spacing");
  Fetch(MDNum::PixelSpacing, imd, "support_data.range_spacing");
  imd.Add(MDStr::SensorID, "SAR");
  auto instrument = m_MetadataSupplierInterface->GetAs<std::string>("sensor");
  instrument.pop_back();
  imd.Add(MDStr::Instrument, instrument);
  Fetch(MDStr::Mode, imd, "header.swath");
  Fetch(MDNum::NumberOfLines, imd, "number_lines", 0);
  Fetch(MDNum::NumberOfColumns, imd, "number_samples", 0);
  Fetch(MDNum::AverageSceneHeight, imd, "support_data.avg_scene_height", 0);
  Fetch(MDNum::RadarFrequency, imd, "support_data.radar_frequency", 0);
  Fetch(MDNum::PRF, imd, "support_data.pulse_repetition_frequency", 0);
  Fetch(MDNum::CalScale, imd, "calibration.absoluteCalibrationConstant", 0);
  SARParam sarParam;
  auto hasSAR = GetSAR(sarParam);
  if (hasSAR)
  {
    imd.Bands[0].Add(MDGeom::SAR, sarParam);
  }
  SARCalib sarCalib;
  sarCalib.calibrationLookupFlag = HasCalibrationLookupDataFlagGeom();
  LoadRadiometricCalibrationData(sarCalib);
  imd.Add(MDGeom::SARCalib, sarCalib);
  
  // Manifect data may not be present in geom file
  CheckFetch(MDStr::BeamMode, imd, "manifest_data.acquisition_mode");
  CheckFetch(MDStr::BeamSwath, imd, "manifest_data.swath");
  CheckFetch(MDStr::Mission, imd, "manifest_data.instrument");
  CheckFetch(MDStr::OrbitDirection, imd, "manifest_data.orbit_pass");
  CheckFetch(MDNum::OrbitNumber, imd, "manifest_data.abs_orbit");
  CheckFetch(MDStr::ProductType, imd, "manifest_data.product_type");
  CheckFetch(MDTime::ProductionDate, imd, "manifest_data.date");
  CheckFetch(MDTime::AcquisitionDate, imd, "manifest_data.image_date");
  CheckFetch("FACILITY_IDENTIFIER", imd, "manifest_data.Processing_system_identifier");
  CheckFetch(MDStr::Swath, imd, "manifest_data.swath");
}

void Sentinel1ImageMetadataInterface::Parse(ImageMetadata & imd)
{
  // Try to fetch the metadata from GDAL Metadata Supplier
  if (m_MetadataSupplierInterface->GetAs<std::string>("", "MISSION_ID") == "S1A" || m_MetadataSupplierInterface->GetAs<std::string>("", "MISSION_ID") == "S1B")
    this->ParseGdal(imd);
  // Try to fetch the metadata from GEOM file
  else if (m_MetadataSupplierInterface->GetAs<std::string>("", "sensor") == "SENTINEL-1A" || m_MetadataSupplierInterface->GetAs<std::string>("", "sensor") == "SENTINEL-1B")
    this->ParseGeom(imd);
  // Failed to fetch the metadata
  else
    otbGenericExceptionMacro(MissingMetadataException,
           << "Not a Sentinel1 product");
}

} // end namespace otb
