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

#${otb-module} will be the name of this module and will not need to be #changed when this module is renamed.

set(${otb-module}Tests
   otbMPIConfigTestDriver.cxx
   otbMPIConfigTest.cxx
)

add_executable(otbMPIConfigTestDriver ${${otb-module}Tests}) 
target_link_libraries(otbMPIConfigTestDriver ${${otb-module}-Test_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_CHRONO_LIBRARY})
otb_module_target_label(otbMPIConfigTestDriver)


# MPI Hello test
otb_add_test_mpi(NAME otbMPIConfigTest
   NBPROCS 2
   COMMAND otbMPIConfigTestDriver otbMPIConfigTest )
