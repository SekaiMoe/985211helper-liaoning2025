#ifdef WEBUI

#include "crow.h"
#include <base.h>  // 引入你现有的头文件

std::string load_html(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return "Failed to load HTML file.";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

namespace webui {
	void start() {
	    crow::SimpleApp app;

	    // 设置用于查询专业的 API 路由
	    CROW_ROUTE(app, "/api/professions/<string>")
	    ([](const std::string& university) {
	        std::vector<std::string> professions;
	        search::listProfessions(university, professions);  // 查询专业列表

	        crow::json::wvalue response;
	        crow::json::wvalue::list professionList;  // 创建 JSON 列表

	        // 将专业名称填入 JSON 响应
	        for (const auto& prof : professions) {
	            professionList.emplace_back(prof);  // 将每个专业添加到列表中
	        }

	        response["professions"] = std::move(professionList);  // 将列表赋值给响应

	        return crow::response(response);
	    });

	    // 默认路由
	    CROW_ROUTE(app, "/")
	    ([]{
	        return load_html("index.html");
	    });
	    try {
	        std::cout << "Crow server is running on http://localhost:8080" << std::endl;
	        app.port(8080).multithreaded().run();
	    } catch (const std::system_error& e) {
	        std::cerr << "Error: Failed to bind to port 8080. The port might be in use." << std::endl;
	        std::cerr << "Details: " << e.what() << std::endl;
	        exit(1);  // 正常退出程序
	    }
	}
}

#endif
