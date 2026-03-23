#include "yaml_parser.h"
#include <yaml-cpp/yaml.h>

std::vector<PipelineConfig> YamlParser::parse(const std::string& file) {
    YAML::Node root = YAML::LoadFile(file);

    std::vector<PipelineConfig> result;

    for (auto p : root["pipelines"]) {
        PipelineConfig cfg;
        cfg.id = p["id"].as<std::string>();

        for (auto e : p["elements"]) {
            ElementConfig ec;
            ec.name = e["name"].as<std::string>();
            ec.type = e["type"].as<std::string>();
            cfg.elements.push_back(ec);
        }

        for (auto l : p["links"]) {
            cfg.links.emplace_back(
                l[0].as<std::string>(),
                l[1].as<std::string>()
            );
        }

        result.push_back(cfg);
    }

    return result;
}