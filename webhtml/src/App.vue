<template>
  <div id="app">
    <h1>查询大学的专业信息</h1>
    
    <!-- 输入大学名称 -->
    <label for="university">请输入大学名称:</label>
    <input v-model="university" placeholder="输入大学名称" />
    <button @click="fetchProfessions">查询专业</button>
    
    <!-- 专业选择 -->
    <div v-if="professions.length">
      <h2>选择专业:</h2>
      <select v-model="selectedProfession">
        <option v-for="profession in professions" :key="profession">{{ profession }}</option>
      </select>
      <button @click="fetchResults">查询</button>
    </div>

    <!-- 显示查询结果 -->
    <h2 v-if="result">查询结果:</h2>
    <p v-if="result">{{ result }}</p>
  </div>
</template>

<script lang="ts">
import { Component, Vue } from "vue-property-decorator";
import axios from "axios";

@Component
export default class App extends Vue {
  university = "";
  professions: string[] = [];
  selectedProfession = "";
  result = "";

  // 查询大学对应的专业
  fetchProfessions() {
    this.professions = [];
    this.selectedProfession = "";
    this.result = "";

    axios.get(`/api/professions/${this.university}`)
      .then(response => {
        this.professions = response.data.professions;
      })
      .catch(error => {
        console.error("Error fetching professions:", error);
        alert("无法获取专业列表，请检查服务器。");
      });
  }

  // 查询大学和专业结果
  fetchResults() {
    axios.get(`/api/search/${this.university}/${this.selectedProfession}`)
      .then(response => {
        this.result = response.data.message;
      })
      .catch(error => {
        console.error("Error fetching results:", error);
        alert("查询失败，请检查服务器。");
      });
  }
}
</script>

<style scoped>
#app {
  margin: 40px;
}
</style>

