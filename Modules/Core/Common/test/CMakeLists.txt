#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

otb_module_test()

set(OTBCommonTests
otbCommonTestDriver.cxx
otbImageRegionTileMapSplitter.cxx
otbImageRegionAdaptativeSplitter.cxx
otbRGBAPixelConverter.cxx
otbRectangle.cxx
otbSystemTest.cxx
otbImageRegionSquareTileSplitter.cxx
otbImageRegionNonUniformMultidimensionalSplitter.cxx
otbConfigurationManagerTest.cxx
otbStandardFilterWatcherNew.cxx
otbStandardOneLineFilterWatcherTest.cxx
otbStandardWriterWatcher.cxx
otbStopwatchTest.cxx
)

add_executable(otbCommonTestDriver ${OTBCommonTests})
target_link_libraries(otbCommonTestDriver ${OTBCommon-Test_LIBRARIES})
otb_module_target_label(otbCommonTestDriver)

# Tests Declaration


otb_add_test(NAME coTvImageRegionTileMapSplitter COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coImageRegionTileMapSplitter.txt
  ${TEMP}/coImageRegionTileMapSplitter.txt
  otbImageRegionTileMapSplitter
  ${TEMP}/coImageRegionTileMapSplitter.txt
  )

otb_add_test(NAME coTvImageRegionAdaptativeSplitterStripSmallStream COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvImageRegionAdaptativeSplitterStripSmallStreamOutput.txt
  ${TEMP}/coTvImageRegionAdaptativeSplitterStripSmallStreamOutput.txt
  otbImageRegionAdaptativeSplitter
  0 0 8000 8003 8000 1000 20
  ${TEMP}/coTvImageRegionAdaptativeSplitterStripSmallStreamOutput.txt
  )

otb_add_test(NAME coTvImageRegionAdaptativeSplitterSquaredTileLargeStream COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvImageRegionAdaptativeSplitterSquaredTileLargeStreamOutput.txt
  ${TEMP}/coTvImageRegionAdaptativeSplitterSquaredTileLargeStreamOutput.txt
  otbImageRegionAdaptativeSplitter
  0 0 8192 8192 2048 2048 8
  ${TEMP}/coTvImageRegionAdaptativeSplitterSquaredTileLargeStreamOutput.txt
  )

otb_add_test(NAME coTvImageRegionAdaptativeSplitterSquaredTileSmallStream COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvImageRegionAdaptativeSplitterSquaredTileSmallStreamOutput.txt
  ${TEMP}/coTvImageRegionAdaptativeSplitterSquaredTileSmallStreamOutput.txt
  otbImageRegionAdaptativeSplitter
  0 0 8192 8192 2048 2048 32
  ${TEMP}/coTvImageRegionAdaptativeSplitterSquaredTileSmallStreamOutput.txt
  )

otb_add_test(NAME coTvImageRegionAdaptativeSplitterStripLargeStream COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvImageRegionAdaptativeSplitterStripLargeStreamOutput.txt
  ${TEMP}/coTvImageRegionAdaptativeSplitterStripLargeStreamOutput.txt
  otbImageRegionAdaptativeSplitter
  0 0 8000 8003 8000 16 8
  ${TEMP}/coTvImageRegionAdaptativeSplitterStripLargeStreamOutput.txt
  )

otb_add_test(NAME coTvImageRegionAdaptativeSplitterShiftedROILargeStream COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvImageRegionAdaptativeSplitterShiftedROILargeStreamOutput.txt
  ${TEMP}/coTvImageRegionAdaptativeSplitterShiftedROILargeStreamOutput.txt
  otbImageRegionAdaptativeSplitter
  1000 1000 4000 4000 2000 2000 5
  ${TEMP}/coTvImageRegionAdaptativeSplitterShiftedROILargeStreamOutput.txt
  )

otb_add_test(NAME coTvImageRegionAdaptativeSplitterShiftedROISmallStream COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvImageRegionAdaptativeSplitterShiftedROISmallStreamOutput.txt
  ${TEMP}/coTvImageRegionAdaptativeSplitterShiftedROISmallStreamOutput.txt
  otbImageRegionAdaptativeSplitter
  1000 1000 4000 4000 2000 2000 10
  ${TEMP}/coTvImageRegionAdaptativeSplitterShiftedROISmallStreamOutput.txt
  )

otb_add_test(NAME coTvImageRegionAdaptativeSplitterMissingLine COMMAND otbCommonTestDriver
  otbImageRegionAdaptativeSplitter
  0 0 1000 1000 1000 1 37
  ${TEMP}/coTvImageRegionAdaptativeSplitterMissingLine.txt
  )

otb_add_test(NAME coTvImageRegionAdaptativeSplitterDivideBlock COMMAND otbCommonTestDriver
  otbImageRegionAdaptativeSplitter
  0 0 10 10 5 5 36
  ${TEMP}/coTvImageRegionAdaptativeSplitterDivideBlock.txt
  )

otb_add_test(NAME coTvRGBAPixelConverter COMMAND otbCommonTestDriver
  otbRGBAPixelConverter
  )

otb_add_test(NAME coTvRectangle COMMAND otbCommonTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/otbRectangleTest.txt
  ${TEMP}/otbRectangleTest.txt
  otbRectangle
  ${TEMP}/otbRectangleTest.txt
  2.12 2.35 12.54 2.45   3. 0.15  1. 1.
  )

otb_add_test(NAME coTuSystemTests_IsA_Methods COMMAND otbCommonTestDriver
  otbSystemTest
  ${OTB_DATA_ROOT}/README-OTB-Data
  ${OTB_DATA_ROOT}
  )

otb_add_test(NAME coTuStopwatchTests COMMAND otbCommonTestDriver
  otbStopwatchTest)

otb_add_test(NAME coTvParseHdfSubsetName COMMAND otbCommonTestDriver
  otbParseHdfSubsetName)

otb_add_test(NAME coTvParseHdfFileName COMMAND otbCommonTestDriver
  otbParseHdfFileName)

otb_add_test(NAME coTvImageRegionSquareTileSplitter COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coImageRegionSquareTileSplitter.txt
  ${TEMP}/coImageRegionSquareTileSplitter.txt
  otbImageRegionSquareTileSplitter
  ${TEMP}/coImageRegionSquareTileSplitter.txt
  )

otb_add_test(NAME coTvImageRegionNonUniformMultidimensionalSplitter COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coImageRegionNonUniformMultidimensionalSplitter.txt
  ${TEMP}/coImageRegionNonUniformMultidimensionalSplitter.txt
  otbImageRegionNonUniformMultidimensionalSplitter
  45 45      # Index
  1000 1500  # Size
  10         # Nb split
  2          # Number of slipt
  ${TEMP}/coImageRegionNonUniformMultidimensionalSplitter.txt
  )

otb_add_test(NAME coTvConfigurationManagerDefault COMMAND otbCommonTestDriver
  otbConfigurationManagerTest
  256)

otb_add_test(NAME coTvConfigurationManagerEnvVar COMMAND otbTestDriver
  --add-before-env OTB_MAX_RAM_HINT "256"
  --add-before-env OTB_DEM_DIRECTORY "/path/to/dem/"
  --add-before-env OTB_GEOID_FILE "/path/to/geoid.file"
  Execute $<TARGET_FILE:otbCommonTestDriver>
  otbConfigurationManagerTest
  256 /path/to/dem/ /path/to/geoid.file)

otb_add_test(NAME coTuStandardFilterWatcherNew COMMAND otbCommonTestDriver
  otbStandardFilterWatcherNew
  ${INPUTDATA}/qb_RoadExtract.img
  )
otb_add_test(NAME coTuStandardOneLineFilterWatcher COMMAND otbCommonTestDriver
  otbStandardOneLineFilterWatcherTest
  ${INPUTDATA}/qb_RoadExtract.img
  )
otb_add_test(NAME coTvStandardWriterWatcher COMMAND otbCommonTestDriver
  otbStandardWriterWatcher
  ${INPUTDATA}/couleurs.tif
  ${TEMP}/coTvStandardWriterWatcherOutput.tif
  20
  )
