#include "yaml_parser.h"
#include "pipeline_manager.h"
#include <gst/gst.h>

void RunServer(PipelineManager& manager);

int main(int argc, char *argv[]) {
    gst_init(&argc, &argv);

    PipelineManager manager;

    auto configs = YamlParser::parse("config/pipelines.yaml");

    for (auto& cfg : configs) {
        manager.add(cfg);
    }

    RunServer(manager);

    return 0;
}