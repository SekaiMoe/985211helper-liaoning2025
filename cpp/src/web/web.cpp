#ifdef WEBUI

#include <base.h>
#include <cctype>

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


std::string url_decode(const std::string& src) {
    std::string dest;
    char ch;
    unsigned int j;  // 使用 unsigned int 而非 size_t
    for (std::string::size_type i = 0; i < src.length(); i++) {
        if (src[i] == '%') {
            sscanf(src.substr(i + 1, 2).c_str(), "%x", &j);
            ch = static_cast<char>(j);
            dest += ch;
            i = i + 2; // Skip over the hex digits
        } else if (src[i] == '+') {
            dest += ' '; // Replace + with space
        } else {
            dest += src[i];
        }
    }
    return dest;
}

namespace webui {
    void start() {
        crow::SimpleApp app;

	CROW_ROUTE(app, "/api/professions/<string>")
	([](const std::string& encodedUniversity) {
	    // 使用自定义的 URL 解码函数
	    std::string university = url_decode(encodedUniversity);

	    //std::cout << "Decoded university: " << university << std::endl;  // 打印解码后的大学名称

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


	CROW_ROUTE(app, "/api/search/<string>/<string>")
	([](const std::string& encodedUniversity, const std::string& encodedProfession) {
	    // 使用自定义的 URL 解码函数
	    std::string university = url_decode(encodedUniversity);
	    std::string profession = url_decode(encodedProfession);

	    // 准备 crow::json::wvalue 对象用于存储结果
	    crow::json::wvalue response;

	    // 调用 search::search 进行查询并传入 response
	    search::search(university, profession, response);

	    // 将 response 返回给前端
	    return crow::response(response);
	});

        CROW_ROUTE(app, "/")
        ([]{
            return load_html("index.html");  // 返回 index.html 文件内容
        });

        try {
            std::cout << "Starting web server..." << std::endl;
            const char* filePath = "./metadata.json";
            view::view(filePath);
            app.port(8080).multithreaded().run();
        } catch (const std::system_error& e) {
            std::cerr << "Error: Failed to bind to port 8080. The port might be in use." << std::endl;
            std::cerr << "Details: " << e.what() << std::endl;
            exit(1);  // 正常退出程序
        }
    }
}

#endif
