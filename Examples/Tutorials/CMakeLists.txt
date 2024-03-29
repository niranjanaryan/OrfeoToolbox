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

project(TutorialsExamples)

add_executable(FilteringPipeline FilteringPipeline.cxx)
target_link_libraries(FilteringPipeline ${OTBCommon_LIBRARIES} ${OTBImageBase_LIBRARIES} ${OTBImageIO_LIBRARIES})

add_executable(HelloWorldOTB HelloWorldOTB.cxx)
target_link_libraries(HelloWorldOTB ${OTBImageBase_LIBRARIES})

add_executable(Multispectral Multispectral.cxx)
target_link_libraries(Multispectral ${OTBCommon_LIBRARIES} ${OTBImageBase_LIBRARIES} ${OTBImageIO_LIBRARIES} ${OTBITK_LIBRARIES})

add_executable(OrthoFusion OrthoFusion.cxx)
target_link_libraries(OrthoFusion ${OTBCommon_LIBRARIES} ${OTBImageBase_LIBRARIES} ${OTBImageIO_LIBRARIES} ${OTBTransform_LIBRARIES} ${OTBProjection_LIBRARIES} ${OTBITK_LIBRARIES})

add_executable(Pipeline Pipeline.cxx)
target_link_libraries(Pipeline ${OTBCommon_LIBRARIES} ${OTBImageBase_LIBRARIES} ${OTBImageIO_LIBRARIES} ${OTBITK_LIBRARIES})

add_executable(ScalingPipeline ScalingPipeline.cxx)
target_link_libraries(ScalingPipeline ${OTBCommon_LIBRARIES} ${OTBImageBase_LIBRARIES} ${OTBImageIO_LIBRARIES} ${OTBITK_LIBRARIES})

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
