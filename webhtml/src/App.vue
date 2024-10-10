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

<script lang="ts" src="./app.ts"></script>

<style scoped>
</style>

