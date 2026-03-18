# Workflow 初学者进度板

## 当前阶段
- 阶段 1：项目总览与第一个异步任务

## 今日已完成
- [x] 编译 tutorial 示例。
- [x] 本地跑通 client + server 闭环（`wget` -> `http_echo_server`）。
- [x] 在 `tutorial-01-wget.cc` 增加学习反馈输出：响应体字节数。
- [x] 修改 `User-Agent` 并验证生效。

## 可见成果（你已经看到）
- 请求头中出现：`User-Agent: WorkflowBeginner/0.1`
- 终端出现：`[Learning] Response body bytes: 142`
- 服务端日志出现：`Peer address: 127.0.0.1:xxxx, seq: 0`

## 下一步（明天）
- [ ] 在 `wget_callback()` 里按 state 分类打印“人类可读错误原因”。
- [ ] 人为构造 DNS 错误，记录 `state/error`。
- [ ] 阅读并标注 `docs/about-error.md` 中你最常用的 3 个状态。

## 复盘问题
- 工厂函数为什么不会返回空指针？
- 为什么 start() 后不建议再直接操作 task 指针？
- server 的 process 与 client 的 callback 各负责什么？
