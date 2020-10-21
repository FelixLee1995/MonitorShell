/**
*  File: ZbxSender.h
*  Desc: zbx上报
*  Created by felix on 2020/09/10
*/
#pragma once

#include <string>
#include <atomic>
#include <queue>
#include <memory>
#include <regex>

#include "Config.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"

#include "asio.hpp"


class ZbxSender
		{
		public:
			explicit ZbxSender(const char * configpath);
			std::atomic<bool> ready;

			void send(std::string data);
	
			void stop();
			~ZbxSender();

			int Check_and_Send(const std::string &);

		ZabbixConfig* p_config_;

		private:
			void run();
			bool tcp_send(std::string value);
		private:
			std::shared_ptr<spdlog::logger> logger;
			std::queue<std::string> queue;
			std::string TextError;
			std::regex regex_;
			char read_buff[1024];
		};
		