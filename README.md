# GStreamer Framework (Config-Driven + gRPC Control)

## 📌 项目简介

本项目是一个基于 **GStreamer + C++ + gRPC** 的流媒体处理框架，核心目标是：

* ✅ **通过配置文件定义 Pipeline（无需写死代码）**
* ✅ **支持 gRPC 远程控制（启动 / 停止）**
* ✅ **具备扩展能力（后续支持插件 / 动态 pipeline）**

👉 当前版本为 **第一版（MVP）**，已具备完整最小闭环能力：

* YAML → Pipeline 构建
* Pipeline 生命周期管理
* gRPC 控制接口
* 可运行 demo（测试视频流）

---

## 🧱 项目结构

```
gstreamer-framework/
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

## ⚙️ 环境依赖

### Ubuntu 安装依赖

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

## 🔧 Protobuf / gRPC 代码生成

进入项目根目录执行：

```bash
protoc -I=proto \
  --grpc_out=./src --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
  --cpp_out=./src \
  proto/pipeline.proto
```

生成文件：

```
src/pipeline.pb.cc
src/pipeline.grpc.pb.cc
```

⚠️ 注意：必须在 `CMakeLists.txt` 中包含 `.cc` 文件，否则会出现链接错误！

---

## 🏗️ 编译项目

```bash
mkdir build
cd build

cmake ..
make -j
```

生成可执行文件：

```
./app
```

---

## ▶️ 运行服务

```bash
./app
```

输出：

```
Server listening on 0.0.0.0:50051
```

---

## 🧪 测试 Pipeline

### 1️⃣ 安装 grpcurl（使用 Go + 软连接）

#### 安装（需要已安装 Go）

```bash
go install github.com/fullstorydev/grpcurl/cmd/grpcurl@latest
```

#### 创建软连接（推荐）

```bash
sudo ln -s $(go env GOPATH)/bin/grpcurl /usr/local/bin/grpcurl
```

#### 验证

```bash
grpcurl -help
```

---

### 2️⃣ 启动 Pipeline

```bash
grpcurl -plaintext \
  -d '{"id":"test_pipeline"}' \
  localhost:50051 pipeline.PipelineService/Start
```

返回：

```json
{
  "success": true
}
```

---

### 3️⃣ 成功效果

👉 屏幕弹出测试画面（彩条）

说明：

* GStreamer pipeline 构建成功
* gRPC 控制正常

---

## 📄 示例配置（pipelines.yaml）

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

## 🔌 gRPC 接口说明

### Service

```
pipeline.PipelineService
```

### 方法

| 方法    | 说明          |
| ----- | ----------- |
| Start | 启动 pipeline |
| Stop  | 停止 pipeline |

---

### 示例调用

#### Start

```bash
grpcurl -plaintext \
  -d '{"id":"test_pipeline"}' \
  localhost:50051 pipeline.PipelineService/Start
```

#### Stop

```bash
grpcurl -plaintext \
  -d '{"id":"test_pipeline"}' \
  localhost:50051 pipeline.PipelineService/Stop
```

---

## 🚨 常见问题

---

### ❌ 1. gRPC 链接错误

```
undefined reference to pipeline::PipelineService
```

✅ 解决：确保 `pipeline.pb.cc` 和 `pipeline.grpc.pb.cc` 被编译

---

### ❌ 2. grpcurl 报错

```
server does not support the reflection API
```

✅ 解决：

方法1（推荐）：启用 reflection
方法2：

```bash
grpcurl -plaintext \
  -import-path ./proto \
  -proto pipeline.proto \
  ...
```

---

---

## 🚀 后续规划

* ✅ YAML 属性（bitrate / url）
* ✅ dynamic pad（decodebin）
* ✅ bus 事件处理（错误 / EOS）
* ✅ 自动重启机制
* ✅ 插件系统（AI节点）

---

## 📌 总结

这是一个：

👉 **配置驱动的 GStreamer 流媒体引擎 + gRPC 控制框架**

已具备工程化基础，可持续扩展为完整流媒体平台 🚀
