/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

#include "otbMetadataStorageInterface.h"

namespace otb
{

void MetadataStorageInterface::SetAs(const std::string & path, double value, int band)
{
  std::ostringstream oss;
  oss << value;
  SetMetadataValue(path.c_str(), oss.str().c_str(), band);
}

void MetadataStorageInterface::SetAs(const std::string & path, const std::string & value, int band)
{
  SetMetadataValue(path.c_str(), value.c_str(), band);
}

}