#pragma once

// 必须定义动态链接宏
#define BOOST_LOG_DYN_LINK 1 

#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp> // 新增控制台输出支持
#include <boost/log/attributes.hpp>  // 新增属性库


//定义
#define LOG_DEBUG BOOST_LOG_TRIVIAL(debug)
#define LOG_INFO BOOST_LOG_TRIVIAL(info)
#define LOG_ERROR BOOST_LOG_TRIVIAL(error)

void initLogger();//启动

