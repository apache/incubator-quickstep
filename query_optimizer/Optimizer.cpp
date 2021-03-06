/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#include "query_optimizer/Optimizer.hpp"

#include "query_optimizer/ExecutionGenerator.hpp"
#include "query_optimizer/LogicalGenerator.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/resolver/Resolver.hpp"

namespace quickstep {
namespace optimizer {

void Optimizer::generateQueryHandle(const ParseStatement &parse_statement,
                                    CatalogDatabase *catalog_database,
                                    OptimizerContext *optimizer_context,
                                    QueryHandle *query_handle) {
  LogicalGenerator logical_generator(optimizer_context);
  PhysicalGenerator physical_generator(optimizer_context);
  ExecutionGenerator execution_generator(catalog_database, query_handle);

  execution_generator.generatePlan(
      physical_generator.generatePlan(
          logical_generator.generatePlan(*catalog_database, parse_statement),
          catalog_database));
}

void Optimizer::findReferencedBaseRelations(const ParseStatement &parse_statement,
                                            CatalogDatabase *catalog_database,
                                            QueryHandle *query_handle) {
  OptimizerContext optimizer_context;
  resolver::Resolver resolver(*catalog_database, &optimizer_context);
  resolver.resolve(parse_statement);
  query_handle->setReferencedBaseRelations(resolver.getReferencedBaseRelations());
}

}  // namespace optimizer
}  // namespace quickstep
