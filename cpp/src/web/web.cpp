#include "crow.h"
#include <base.h>  // 引入你现有的头文件

namespace webui {
	void start() {
	    crow::SimpleApp app;

	    // 设置路由，用于获取专业列表
	    CROW_ROUTE(app, "/api/professions/<string>")
	    ([](const std::string& university) {
	        // 调用现有的函数来获取专业列表
	        std::vector<std::string> professions;
	        search::listProfessions(university, professions);  // 查询专业

	        crow::json::wvalue response;
	        response["professions"] = crow::json::wvalue::list();  // 使用正确的 API 创建 JSON 列表

        	// 将专业名称填入 JSON 响应
	        for (const auto& prof : professions) {
	            response["professions"].push_back(prof);  // 将每个专业添加到 JSON 列表
	        }

	        return crow::response(response);
	    });

	    std::cout << "Crow server is running on http://localhost:8080" << std::endl;
	    app.port(8080).multithreaded().run();
	}
}
