#ifdef WEBUI

#include "crow.h"
#include <base.h>  // 引入你现有的头文件

std::string load_html(const std::string& filepath) {
    const char* filePath = "./metadata.json";
    view::view(filePath);
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

	    // 1. 查询大学对应的专业列表
	    CROW_ROUTE(app, "/api/professions/<string>")
	    ([](const std::string& university) {
	        std::vector<std::string> professions;
	        search::listProfessions(university, professions);  // 查询专业列表

	        crow::json::wvalue response;
	        crow::json::wvalue::list professionList;

	        for (const auto& prof : professions) {
	            professionList.emplace_back(prof);  // 将专业添加到列表中
	        }

	        response["professions"] = std::move(professionList);
	        return crow::response(response);
	    });

	    // 2. 根据大学和专业进行精确查询
	    CROW_ROUTE(app, "/api/search/<string>/<string>")
	    ([](const std::string& university, const std::string& profession) {
	        // 调用 search::search 进行查询
	        search::search(university, profession);

	        crow::json::wvalue response;
	        response["message"] = "查询成功！";
	        return crow::response(response);
	    });

	    // 提供静态 HTML 文件
	    CROW_ROUTE(app, "/")
	    ([]{
	        return load_html("index.html");  // 返回 index.html 文件内容
	    });

	    try {
	        std::cout << "Starting web server..." << std::endl;
	        app.port(8080).multithreaded().run();
	    } catch (const std::system_error& e) {
	        std::cerr << "Error: Failed to bind to port 8080. The port might be in use." << std::endl;
	        std::cerr << "Details: " << e.what() << std::endl;
	        exit(1);  // 正常退出程序
	    }
	}
}

#endif
