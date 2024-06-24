<template>
  <div class="container">
    <div v-for="(step, index) in steps" :key="index" class="row my-4">
      <div class="col-md-4">
        <div class="d-flex flex-column justify-content-center h-100">
          <h4 class="mb-2 text-center">{{ "Step " + (index + 1) }}</h4>
          <div class="text-center" v-html="step.description"></div>
        </div>
      </div>
      <div class="col-md-8">
        <img :src="step.image" :alt="'Step ' + (index + 1)" class="img-fluid" />
      </div>
    </div>
  </div>
</template>

<script lang="ts" setup>
import { ref } from "vue";
import Tutorial from "~/assets/tutorial.json";

interface Step {
  image: string;
  description: string;
}

const steps = ref<Step[]>(Tutorial);
</script>

<style scoped>
img {
  max-height: 1000px;
}

.row {
  overflow-x: hidden;
}
</style>
