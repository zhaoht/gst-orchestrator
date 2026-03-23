#include "pipeline_builder.h"
#include <unordered_map>
#include <iostream>

GstElement* PipelineBuilder::build(const PipelineConfig& config) {
    GstElement* pipeline = gst_pipeline_new(config.id.c_str());

    std::unordered_map<std::string, GstElement*> elems;

    for (auto& e : config.elements) {
        GstElement* elem = gst_element_factory_make(e.type.c_str(), e.name.c_str());
        if (!elem) {
            std::cerr << "Failed to create element: " << e.type << std::endl;
            return nullptr;
        }

        gst_bin_add(GST_BIN(pipeline), elem);
        elems[e.name] = elem;
    }

    for (auto& link : config.links) {
        gst_element_link(elems[link.first], elems[link.second]);
    }

    return pipeline;
}