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

<script>
import { ref, onMounted, watch } from "vue";

export default {
  name: "Card",
  props: {
    service: Object,
  },
  data() {
    return {
      progress: 100,
      intervalId: null,
      isOverflowing: false,
      code: null,
    };
  },
  methods: {
    startCountdown() {
      this.intervalId = setInterval(() => {
        const now = new Date();
        const totalSeconds = now.getSeconds() + now.getMilliseconds() / 1000;
        const remainingSeconds = 30 - (totalSeconds % 30);
        if (remainingSeconds < 1) {
          this.progress = 100;
          this.code = this.generateRandomCode();
        } else {
          this.progress = (remainingSeconds / 30) * 100;
        }
      }, 1000);
    },
    generateRandomCode() {
      return Math.floor(100000 + Math.random() * 900000);
    },
  },
  mounted() {
    this.code = this.generateRandomCode();
    this.startCountdown();
  },
  beforeUnmount() {
    clearInterval(this.intervalId);
  },
};
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
