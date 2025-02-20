/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <Processors/QueryPlan/ReadFromPreparedSource.h>
#include <Processors/QueryPlan/SourceStepWithFilter.h>
#include <Storages/MergeTree/KeyCondition.h>
#include <Interpreters/Context_fwd.h>
#include <Core/NamesAndTypes.h>

namespace local_engine
{
class SubstraitFileSourceStep : public DB::SourceStepWithFilter
{
public:
    explicit SubstraitFileSourceStep(DB::ContextPtr context_, DB::Pipe pipe_, const String & name);

    void applyFilters() override;

    String getName() const override { return "SubstraitFileSourceStep"; }

    void initializePipeline(DB::QueryPipelineBuilder &, const DB::BuildQueryPipelineSettings &) override;

private:
    DB::Pipe pipe;
    DB::ContextPtr context;
    std::vector<DB::String> partition_keys;
    DB::String file_format;

    DB::NamesAndTypesList extractParquetFileColumnPathAndTypeForComplexType(const DB::String & column_name, const DB::DataTypePtr & column_type);
};

struct SourceFilter
{
    DB::KeyCondition filter;
    DB::NamesAndTypesList keys;
};

}

