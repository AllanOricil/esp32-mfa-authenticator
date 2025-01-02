<template>
  <div class="card">
    <div class="row">
      <div class="col">
        <div class="marquee">
          <div>
            <span>{{ service.name }}</span>
            <span>{{ service.name }}</span>
          </div>
        </div>
      </div>
    </div>

    <div class="row">
      <div class="col">
        <div class="code">{{ code }}</div>
      </div>
    </div>

    <div class="row">
      <div class="col">
        <div class="progress">
          <div
            class="progress-bar"
            role="progressbar"
            :style="{ width: progress + '%' }"
          ></div>
        </div>
      </div>
    </div>
  </div>
</template>

<script lang="ts" setup>
import { ref, onMounted, onBeforeUnmount } from "vue";

defineProps({
  service: Object,
});

const progress = ref(100);
const intervalId = ref<NodeJS.Timeout | null>(null);
const isOverflowing = ref(false);
const code = ref<number | null>(null);

const startCountdown = () => {
  intervalId.value = setInterval(() => {
    const now = new Date();
    const totalSeconds = now.getSeconds() + now.getMilliseconds() / 1000;
    const remainingSeconds = 30 - (totalSeconds % 30);
    if (remainingSeconds < 1) {
      progress.value = 100;
      code.value = generateRandomCode();
    } else {
      progress.value = (remainingSeconds / 30) * 100;
    }
  }, 1000);
};

const generateRandomCode = (): number => {
  return Math.floor(100000 + Math.random() * 900000);
};

onMounted(() => {
  code.value = generateRandomCode();
  startCountdown();
});

onBeforeUnmount(() => {
  if (intervalId.value) {
    clearInterval(intervalId.value);
  }
});
</script>

<style scoped>
.card {
  display: flex;
  flex-direction: column;
  padding: 20px;
  border: 1px solid #252525;
  border-radius: 8px;
  background-color: #2b2b2b;
  overflow: hidden;
  height: 100%;
}

.code {
  font-size: 35px;
}

.progress {
  height: 5px;
  background-color: #ddd;
}

.progress-bar {
  height: 100%;
  background-color: #007bff;
}

.marquee {
  height: 25px;
  overflow: hidden;
  position: relative;
}

.marquee div {
  display: block;
  width: 200%;
  height: 30px;
  position: absolute;
  overflow: hidden;
  animation: marquee 5s linear infinite;
}

.marquee span {
  float: left;
  width: 50%;
}

@keyframes marquee {
  0% {
    left: 0;
  }
  100% {
    left: -100%;
  }
}
</style>
