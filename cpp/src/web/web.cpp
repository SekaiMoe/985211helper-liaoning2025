#ifdef WEBUI

#include <base.h>
#include <cctype>

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

    const std::string embedded_html = R"(
	<!DOCTYPE html>
	<html lang="en">
	<head>
	    <meta charset="UTF-8">
	    <meta name="viewport" content="width=device-width, initial-scale=1.0">
	    <title>Test html</title>
    	    <style>
                /* Fluent Design 样式 */
                body {
                    background: linear-gradient(to right, #0078D7, #00B4FF);
                    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
                    color: #ffffff;
                    margin: 0;
                    padding: 0;
                    display: flex;
                    justify-content: center;
                    align-items: center;
                    height: 100vh;
                }
                #app {
                    background: rgba(255, 255, 255, 0.1);
                    border-radius: 12px;
                    padding: 20px;
                    box-shadow: 0 4px 30px rgba(0, 0, 0, 0.1);
                    backdrop-filter: blur(10px);
                    -webkit-backdrop-filter: blur(10px);
                    width: 80%;
                    max-width: 600px;
                }
                h1 {
                    text-align: center;
                    font-size: 24px;
                }
                label, select, input, button {
                    display: block;
                    width: 100%;
                    margin-bottom: 15px;
                    font-size: 16px;
                }
                input, select {
                    padding: 10px;
                    border-radius: 8px;
                    border: none;
                    background: rgba(255, 255, 255, 0.2);
                    color: #fff;
                }
                button {
                    padding: 12px;
                    background-color: #0078D7;
                    border: none;
                    border-radius: 8px;
                    color: #fff;
                    font-weight: bold;
                    cursor: pointer;
                    transition: background-color 0.3s ease;
                }
                button:hover {
                    background-color: #005a9e;
                }
                h2 {
                    margin-top: 20px;
                    font-size: 20px;
                }
                p {
                    line-height: 1.5;
                    white-space: pre-wrap;
                }
            </style>
	    <script src="https://cdn.jsdelivr.net/npm/vue@2.6.14"></script>
	    <script src="https://cdn.jsdelivr.net/npm/axios/dist/axios.min.js"></script>
	</head>
	<body>
	    <div id="app">
	        <h4>专业分数线自助查询系统V0.0.1</h4>

	        <!-- 输入大学名称 -->
	        <label for="university">请输入大学名称:</label>
	        <input type="text" v-model="university" placeholder="输入大学名称">
	        <button @click="fetchProfessions">查询专业</button>

	        <!-- 显示专业选择 -->
	        <div v-if="professions.length">
	            <h2>选择专业:</h2>
	            <select v-model="selectedProfession">
	                <option v-for="profession in professions" :key="profession">{{ profession }}</option>
	            </select>
	            <button @click="fetchResults" :disabled="!selectedProfession">查询</button>
	        </div>

	        <!-- 显示查询结果 -->
	        <h2 v-if="result">查询结果:</h2>
	        <p v-if="result" v-html="result"></p>

	    </div>
	<script>
	    new Vue({
	        el: '#app',
	        data: {
	            university: '',  // 用户输入的大学名称
	            professions: [],  // 专业列表
	            selectedProfession: '',  // 用户选择的专业
	            result: '',  // 查询结果
	            searched: false  // 是否已进行查询
	        },
	        methods: {
	            fetchProfessions() {
	                // 重置
	                this.professions = [];
        	        this.selectedProfession = '';
	                this.result = '';

	                // 请求大学对应的专业列表
	                axios.get(`/api/professions/${this.university}`)
	                    .then(response => {
        	                this.professions = response.data.professions;
	                        this.searched = true;
	                    })
	                    .catch(error => {
	                        console.error("Error fetching professions:", error);
	                        alert("无法获取专业列表，请检查服务器。");
	                    });
	            },
	            fetchResults() {
	                // 查询大学和选择的专业
	                axios.get(`/api/search/${this.university}/${this.selectedProfession}`)
	                    .then(response => {
	                        console.log("Response data:", response.data);
	                        // 根据返回的数据更新结果并插入换行符
	                        this.result = `
        	                    学校: ${response.data.university}<br>
	                            专业: ${response.data.profession}<br>
	                            最低投档分数:<br>
	                            ${response.data.scores.map(score => `Year ${score.year}: ${score.score}`).join('<br>')}<br>
	                            近三年平均分(向上取整): ${response.data.average_score}<br>
	                            Year 2025(仅供参考): ${response.data.predicted_2025_score}<br>
	                            该学校是否是985: ${response.data.is985}<br>
	                            该学校是否是211: ${response.data.is211}
        	                `;
	                    })
	                    .catch(error => {
	                        console.error("Error fetching results:", error);
	                        alert("查询失败，请检查服务器。");
	                    });
	            }
	        }
	    });
	</script>

	</body>
	</html>
    )";

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
            return embedded_html;
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
