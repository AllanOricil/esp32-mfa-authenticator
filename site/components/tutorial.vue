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

interface Step {
  image: string;
  description: string;
}

const steps = ref<Step[]>([
  {
    image: "/tutorial/step-1.png",
    description: "Click on the <b>Install</b> button",
  },
  {
    image: "/tutorial/step-2.png",
    description: "Select the serial port that belongs to your board",
  },
  {
    image: "/tutorial/step-3.png",
    description: "Select <b>Install esp32-mfa-authenticator</b>",
  },
  {
    image: "/tutorial/step-4.png",
    description:
      "Select <b>Erase device</b> and then click on the <b>Next</b> button",
  },
  {
    image: "/tutorial/step-5.png",
    description: "Click on the <b>Install</b> button",
  },
  {
    image: "/tutorial/step-6.png",
    description: "Wait for the instalation to finish",
  },
  {
    image: "/tutorial/step-7.png",
    description:
      "Click on the <b>Next</b> button when the instalation is complete",
  },
  {
    image: "/tutorial/step-8.png",
    description: "Select <b>Logs & Console</b>",
  },
  {
    image: "/tutorial/step-9.png",
    description:
      "Calibrate the touch sensor when prompted. If the calibration phase has already passed when you opened the logs screen, click on the Reset(RST) button on the board",
  },
]);
</script>

<style scoped>
img {
  max-height: 1000px;
}

.row {
  overflow-x: hidden;
}
</style>
