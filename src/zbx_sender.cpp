#include "ZbxSender.h"
#include "CByteOrder.h"

ZbxSender::ZbxSender(const char * configpath)
{

	p_config_ = new ZabbixConfig(configpath);

    regex_ = std::regex(p_config_->GetZbx_Submit_Rule());

	logger = spdlog::daily_logger_mt("daily_logger", "zbx_monitor.log", 2, 30);
    logger->set_level(spdlog::level::debug);

    logger->flush_on(spdlog::level::info);


    logger->set_pattern("[%D %H:%M:%S.%f ] [%t] [%s:%#(%!)] %v");

    spdlog::flush_every(std::chrono::seconds(1));

    spdlog::set_default_logger(logger);


	ready = true;
	std::thread worker(&ZbxSender::run, this);
	worker.detach();
}

void ZbxSender::send(std::string data)
{
	queue.push(data);
}



void ZbxSender::stop()
{
	ready = false;
}

ZbxSender::~ZbxSender()
{
	stop();
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

void ZbxSender::run()
{
	SPDLOG_INFO("zabbixSender started");
	while (ready)
	{
		if (!queue.empty())
		{
			std::string data = queue.front();
			queue.pop();
			if (data != "")
			{
				tcp_send(data);
			}
		}
		if (queue.size() > 100)
		{
			queue.empty();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	SPDLOG_INFO("zabbixSender exit");
}

bool ZbxSender::tcp_send(std::string value)
{
	asio::io_service io_service;
	asio::ip::tcp::socket socket(io_service);
	try
	{
        SPDLOG_INFO("host: {}, port: {}", p_config_->GetZbx_server(), p_config_->GetZbxPort());
		socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string(p_config_->GetZbx_server()),
			p_config_->GetZbxPort()));
	}
	catch (std::exception& e)
	{
		SPDLOG_WARN("exception: {}", e.what());
		return false;
	}

	if (value.length() > 4096)
	{
		SPDLOG_ERROR("msg is too long:{}", value);
		return false;
	}

	nlohmann::json j;
    j["request"] = "sender data";

    nlohmann::json j1;
    j1["host"] = p_config_->GetZbxHostName();
    j1["key"] = p_config_->GetZbxKey();
    j1["value"] = value;
    j["data"].push_back(j1);

    auto jsonstr = j.dump();
    unsigned int len = jsonstr.length();

    char buff_[1024];
    memset(buff_, 0x00, 1024);

    memcpy(buff_, "ZBXD", 4);
    buff_[4] = 0x01;
 
    memcpy(buff_ + 13, jsonstr.c_str(), jsonstr.length());
    memcpy(buff_ + 5, &len, 4);

	asio::error_code error;
	asio::write(socket, asio::buffer(buff_, jsonstr.length() + 13), error);

	if (error)
	{
		TextError = "send failed: " + error.message();
		SPDLOG_ERROR("{} data detail:{}", TextError, value);
	}
	else
	{
		SPDLOG_INFO("send:{}", value);
	}

	asio::streambuf receive_buffer;
	asio::read(socket, receive_buffer, asio::transfer_all(), error);

	if (error && error != asio::error::eof)
	{
		SPDLOG_ERROR("receive failed: {}", error.message());
	}
	else
	{
		const char* data = asio::buffer_cast<const char*>(receive_buffer.data());
		SPDLOG_INFO("receive data:{}", data);
	}

    SPDLOG_INFO("zabbix send end");
	return true;
}


int ZbxSender::Check_and_Send(const std::string& str) {
    std::smatch res;
    auto ret = std::regex_search(str.begin(), str.end(), res, std::regex(regex_));

    if (ret) {
        send(str);
    }

    return ret ? 0 : -1;
}

