#pragma once
#include <gst/gst.h>
#include <string>
#include <vector>
#include <map>

struct ElementConfig {
    std::string name;
    std::string type;
};

struct PipelineConfig {
    std::string id;
    std::vector<ElementConfig> elements;
    std::vector<std::pair<std::string, std::string>> links;
};

class PipelineBuilder {
public:
    GstElement* build(const PipelineConfig& config);
};