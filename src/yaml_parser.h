#pragma once
#include "pipeline_builder.h"
#include <vector>
#include <string>

class YamlParser {
public:
    static std::vector<PipelineConfig> parse(const std::string& file);
};