#include "logger.h"

namespace logging=boost::log;


void initLogger(){

    //核心配置
    logging::core::get()->remove_all_sinks();


    

    //文件输出配置
    logging::add_file_log(
        logging::keywords::file_name="logs/chat_%N.log",
        logging::keywords::rotation_size=10*1024*1024,//达到10MB时换新文件
        logging::keywords::max_size=100*1024*1024,//
        logging::keywords::format="[%TimeStamp%] <%Severity%> ------%Message%",
        logging::keywords::auto_flush=true   
    );

    //控制台输出
    auto console_sink=logging::add_console_log(
        std::cout,
        logging::keywords::format="[%TimeStamp%] <%Severity%> %Message%"

    );
    
    //通用属性
    logging::add_common_attributes();
    logging::core::get()->add_global_attribute("Scope",logging::attributes::named_scope());//添加作用域


    //设置过滤级别-----监控级别
    logging::core::get()->set_filter(
        logging::trivial::severity  >= logging::trivial::info
    );

    //验证日志
    LOG_INFO<<"Logger 初始化成功！！！！！！！！！！！！！";

}
