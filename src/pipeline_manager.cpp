#include "pipeline_manager.h"

void PipelineManager::add(const PipelineConfig& cfg) {
    PipelineBuilder builder;
    GstElement* p = builder.build(cfg);

    pipelines_[cfg.id] = {p};
}

bool PipelineManager::start(const std::string& id) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = pipelines_.find(id);
    if (it == pipelines_.end()) return false;

    gst_element_set_state(it->second.pipeline, GST_STATE_PLAYING);
    return true;
}

bool PipelineManager::stop(const std::string& id) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = pipelines_.find(id);
    if (it == pipelines_.end()) return false;

    gst_element_set_state(it->second.pipeline, GST_STATE_NULL);
    return true;
}