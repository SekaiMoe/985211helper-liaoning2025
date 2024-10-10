import { defineComponent, ref } from 'vue';
import axios from 'axios';

export default defineComponent({
  name: 'App',
  setup() {
    const university = ref('');
    const professions = ref<string[]>([]);
    const selectedProfession = ref('');
    const result = ref('');

    const fetchProfessions = async () => {
      professions.value = [];
      selectedProfession.value = '';
      result.value = '';

      try {
        const response = await axios.get(`/api/professions/${encodeURIComponent(university.value)}`);
        professions.value = response.data.professions;
      } catch (error) {
        console.error("Error fetching professions:", error);
        alert("无法获取专业列表，请检查服务器。");
      }
    };

    const fetchResults = async () => {
      try {
        const response = await axios.get(`/api/search/${encodeURIComponent(university.value)}/${encodeURIComponent(selectedProfession.value)}`);
        result.value = response.data.message;
      } catch (error) {
        console.error("Error fetching results:", error);
        alert("查询失败，请检查服务器。");
      }
    };

    return {
      university,
      professions,
      selectedProfession,
      result,
      fetchProfessions,
      fetchResults
    };
  }
});

