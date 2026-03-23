#include "pipeline.grpc.pb.h"
#include "pipeline_manager.h"
#include <grpcpp/grpcpp.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class PipelineServiceImpl final : public pipeline::PipelineService::Service {
public:
    PipelineServiceImpl(PipelineManager& m) : manager_(m) {}

    Status Start(ServerContext*, const pipeline::StartRequest* req,
                 pipeline::CommonReply* rep) override {
        bool ok = manager_.start(req->id());
        rep->set_success(ok);
        return Status::OK;
    }

    Status Stop(ServerContext*, const pipeline::StopRequest* req,
                pipeline::CommonReply* rep) override {
        bool ok = manager_.stop(req->id());
        rep->set_success(ok);
        return Status::OK;
    }

private:
    PipelineManager& manager_;
};

void RunServer(PipelineManager& manager) {
    std::string addr("0.0.0.0:50051");

    PipelineServiceImpl service(manager);

    ServerBuilder builder;
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << addr << std::endl;

    server->Wait();
}