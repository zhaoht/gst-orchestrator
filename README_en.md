# GStreamer Framework (Config-Driven + gRPC Control)

## 📌 Overview

This project is a **GStreamer-based streaming framework** built with **C++ + gRPC**, designed to provide:

* ✅ **Fully configurable pipelines (no hardcoded logic)**
* ✅ **Remote control via gRPC (start / stop pipelines)**
* ✅ **Extensibility (future support for plugins and dynamic pipelines)**

👉 The current version is **MVP (v1)** and already provides a complete minimal working system:

* YAML → Pipeline construction
* Pipeline lifecycle management
* gRPC control interface
* Runnable demo (video test stream)

---

## 🧱 Project Structure

```
gst-orchestrator/
├── CMakeLists.txt
├── config/
│   └── pipelines.yaml
├── proto/
│   └── pipeline.proto
├── src/
│   ├── main.cpp
│   ├── pipeline_manager.*
│   ├── pipeline_builder.*
│   ├── yaml_parser.*
│   ├── grpc_server.cpp
│   ├── pipeline.pb.cc
│   └── pipeline.grpc.pb.cc
```

---

## ⚙️ Dependencies

### Install on Ubuntu

```bash
sudo apt update

sudo apt install -y \
    libgstreamer1.0-dev \
    libgstreamer-plugins-base1.0-dev \
    libyaml-cpp-dev \
    libgrpc++-dev \
    protobuf-compiler-grpc \
    cmake \
    build-essential
```

---

## 🔧 Protobuf / gRPC Code Generation

Run the following in the project root:

```bash
protoc -I=proto \
  --grpc_out=./src --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
  --cpp_out=./src \
  proto/pipeline.proto
```

Generated files:

```
src/pipeline.pb.cc
src/pipeline.grpc.pb.cc
```

⚠️ Important:
Make sure `.cc` files are included in `CMakeLists.txt`, otherwise you will get linker errors.

---

## 🏗️ Build

```bash
mkdir build
cd build

cmake ..
make -j
```

Executable:

```
./app
```

---

## ▶️ Run

```bash
./app
```

Output:

```
Server listening on 0.0.0.0:50051
```

---

## 🧪 Testing Pipelines

### 1️⃣ Install grpcurl (via Go + symlink)

#### Install (requires Go)

```bash
go install github.com/fullstorydev/grpcurl/cmd/grpcurl@latest
```

#### Create symlink

```bash
sudo ln -s $(go env GOPATH)/bin/grpcurl /usr/local/bin/grpcurl
```

#### Verify

```bash
grpcurl -help
```

---

### 2️⃣ Start Pipeline

```bash
grpcurl -plaintext \
  -import-path ./proto \
  -proto pipeline.proto \
  -d '{"id":"test_pipeline"}' \
  localhost:50051 pipeline.PipelineService/Start
```

Response:

```json
{
  "success": true
}
```

---

### 3️⃣ Expected Result

👉 A test video window (color bars) will appear

This means:

* GStreamer pipeline is working
* gRPC control is functioning correctly

---

## 📄 Example Configuration (pipelines.yaml)

```yaml
pipelines:
  - id: test_pipeline
    auto_start: false

    elements:
      - name: src
        type: videotestsrc

      - name: conv
        type: videoconvert

      - name: sink
        type: autovideosink

    links:
      - [src, conv]
      - [conv, sink]
```

---

## 🔌 gRPC API

### Service

```
pipeline.PipelineService
```

### Methods

| Method | Description      |
| ------ | ---------------- |
| Start  | Start a pipeline |
| Stop   | Stop a pipeline  |

---

### Example Calls

#### Start

```bash
grpcurl -plaintext \
  -import-path ./proto \
  -proto pipeline.proto \
  -d '{"id":"test_pipeline"}' \
  localhost:50051 pipeline.PipelineService/Start
```

#### Stop

```bash
grpcurl -plaintext \
  -import-path ./proto \
  -proto pipeline.proto \
  -d '{"id":"test_pipeline"}' \
  localhost:50051 pipeline.PipelineService/Stop
```

---

## 🚨 Common Issues

---

### ❌ 1. gRPC Linker Error

```
undefined reference to pipeline::PipelineService
```

✅ Fix:

Make sure these files are compiled:

```
pipeline.pb.cc
pipeline.grpc.pb.cc
```

---

## 🚀 Roadmap

Next versions will include:

* ✅ YAML properties (bitrate, URL, etc.)
* ✅ Dynamic pad support (decodebin)
* ✅ Bus event handling (error / EOS)
* ✅ Auto-restart mechanism
* ✅ Plugin system (AI / custom elements)

---

## 📌 Summary

This project is:

👉 **A configuration-driven GStreamer streaming engine with gRPC control**

It already provides a solid foundation and can evolve into a full-featured streaming platform 🚀
