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
#----------- Rasterization TESTS ----------------
otb_test_application(NAME apTvRsRasterizationLinesAttributeMode
                     APP  Rasterization
                     OPTIONS -in   LARGEINPUT{VECTOR/MidiPyrenees/roads.shp}
                             -im   ${BASELINE}/apTvRsRasterizationLines.tif
                             -out  ${TEMP}/apTvRsRasterizationLinesAttributeMode.tif uint32
                             -background 0
                             -mode attribute
                             -mode.attribute.field osm_id
                     VALID   --compare-image ${NOTOL}
                              ${BASELINE}/apTvRsRasterizationLinesAttributeMode.tif
                              ${TEMP}/apTvRsRasterizationLinesAttributeMode.tif)

otb_test_application(NAME apTvRsRasterizationPolygons2LayersWith2SRS
                     APP  Rasterization
                     OPTIONS -in  ${INPUTDATA}/ToulouseRoad-examples_2layers_2srs.sqlite
                             -spx 1.
                             -spy -1.
                             -out ${TEMP}/apTvRsRasterizationPolygons2LayersWith2SRS.tif uint8
                             -background 255
                             -mode.binary.foreground 0
                     VALID   --compare-image ${NOTOL}
                              ${BASELINE}/apTvRsRasterizationPolygons2LayersWith2SRS.tif
                	      ${TEMP}/apTvRsRasterizationPolygons2LayersWith2SRS.tif)

otb_test_application(NAME apTvRsRasterizationLines
                     APP  Rasterization
                     OPTIONS -in   LARGEINPUT{VECTOR/MidiPyrenees/roads.shp}
                             -im   ${BASELINE}/apTvRsRasterizationLines.tif
                             -out  ${TEMP}/apTvRsRasterizationLines.tif uint8
                             -background 255
                             -mode.binary.foreground 0
                     VALID   --compare-image ${NOTOL}
                              ${BASELINE}/apTvRsRasterizationLines.tif
                			  ${TEMP}/apTvRsRasterizationLines.tif)

otb_test_application(NAME apTvRsRasterizationLinesStreaming
                     APP  Rasterization
                     OPTIONS -in   LARGEINPUT{VECTOR/MidiPyrenees/roads.shp}
                             -im   ${BASELINE}/apTvRsRasterizationLines.tif
                             -out  ${TEMP}/apTvRsRasterizationLinesStreaming.tif uint8
                 	     -ram 1
                             -background 255
                             -mode.binary.foreground 0
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvRsRasterizationLines.tif
                	     ${TEMP}/apTvRsRasterizationLinesStreaming.tif)

otb_test_application(NAME apTvRsRasterizationPolygons
                     APP  Rasterization
                     OPTIONS -in  ${INPUTDATA}/Rasterization/polgons.shp
                             -spx 1.
                             -spy 1.
                             -out ${TEMP}/apTvRsRasterizationPolygons.tif uint8
                             -background 255
                             -mode.binary.foreground 0
                     VALID   --compare-image ${NOTOL}
                              ${BASELINE}/apTvRsRasterizationPolygons.tif
                	      ${TEMP}/apTvRsRasterizationPolygons.tif)

otb_test_application(NAME apTvRsRasterizationPoints
                     APP  Rasterization
                     OPTIONS -in   LARGEINPUT{VECTOR/MidiPyrenees/points.shp}
                             -im   ${BASELINE}/apTvRsRasterizationPoints.tif
                             -out  ${TEMP}/apTvRsRasterizationPoints.tif uint8
                             -background 255
                             -mode.binary.foreground 0
                     VALID   --compare-image ${NOTOL}
                              ${BASELINE}/apTvRsRasterizationPoints.tif
                	      ${TEMP}/apTvRsRasterizationPoints.tif)

otb_test_application(NAME apTvRsRasterizationShadowsSHP
                     APP  Rasterization
                     OPTIONS -in   ${INPUTDATA}/Capitole-Shadows.shp
                             -im   ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                             -out  ${TEMP}/apTvRsRasterizationShadowsSHP.tif
                             -background 255
                             -mode.binary.foreground 0
                     VALID   --compare-image ${NOTOL}
                              ${BASELINE}/apTvRsRasterizationShadowsSHP.tif
                	            ${TEMP}/apTvRsRasterizationShadowsSHP.tif)

otb_test_application(NAME apTvRsRasterizationShadowsKML
                     APP  Rasterization
                     OPTIONS -in   ${INPUTDATA}/Capitole-Shadows.kml
                             -im   ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                             -out  ${TEMP}/apTvRsRasterizationShadowsKML.tif
                             -background 255
                             -mode.binary.foreground 0
                     VALID   --compare-image ${NOTOL}
                              ${BASELINE}/apTvRsRasterizationShadowsKML.tif
                	            ${TEMP}/apTvRsRasterizationShadowsKML.tif)

