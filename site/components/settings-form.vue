<template>
  <!-- TODO: move toast to layout -->
  <div
    id="submit-toast"
    class="toast align-items-center border-0"
    role="alert"
    aria-live="assertive"
    aria-atomic="true"
  >
    <div class="d-flex">
      <div class="toast-body" :class="toastClass">
        {{ toastMessage }}
      </div>
      <button
        type="button"
        class="btn-close me-2 m-auto"
        data-bs-dismiss="toast"
        aria-label="Close"
      ></button>
    </div>
  </div>
  <form class="space-y-4" @submit.prevent="onSubmit">
    <div class="mb-4">
      <h4 class="mb-3">Wi-Fi</h4>

      <div>
        <label for="ssid" class="form-label">SSID</label>
        <input
          type="text"
          class="form-control"
          id="ssid"
          v-model="state.settings.wifi.ssid"
        />
      </div>
      <div>
        <label for="password" class="form-label">Password</label>
        <input
          type="password"
          class="form-control"
          id="password"
          v-model="state.settings.wifi.password"
        />
      </div>
    </div>
    <div class="mb-4">
      <h4 class="mb-3">MQTT</h4>

      <div>
        <label for="server" class="form-label">Server</label>
        <input
          type="text"
          class="form-control"
          id="server"
          v-model="state.settings.mqtt.server"
        />
      </div>
      <div>
        <label for="port" class="form-label">Port</label>
        <input
          type="number"
          class="form-control"
          id="port"
          min="0"
          @input="ensureNumber"
          v-model.number="state.settings.mqtt.port"
        />
      </div>
      <div>
        <label for="username" class="form-label">Username</label>
        <input
          type="text"
          class="form-control"
          id="username"
          v-model="state.settings.mqtt.username"
        />
      </div>
      <div>
        <label for="password" class="form-label">Password</label>
        <input
          type="password"
          class="form-control"
          id="password"
          v-model="state.settings.mqtt.password"
        />
      </div>
    </div>

    <div class="mb-4">
      <h4 class="mb-3">Security</h4>
      <label for="max-number-of-wrong-unlock-attempts" class="form-label"
        >Max number of wrong unlock attempts</label
      >
      <input
        type="number"
        class="form-control"
        id="max-number-of-wrong-unlock-attempts"
        min="1"
        v-model.number="state.settings.security.maxNumberOfWrongUnlockAttempts"
      />
      <div>
        <label for="pin-hash" class="form-label">Pin Hash</label>
        <input
          type="password"
          class="form-control"
          id="pin-hash"
          v-model="state.settings.security.pin.hash"
        />
      </div>
      <div>
        <label for="pin-key" class="form-label">Pin Key</label>
        <input
          type="password"
          class="form-control"
          id="pin-key"
          v-model="state.settings.security.pin.key"
        />
      </div>
    </div>

    <div class="mb-4">
      <h4 class="mb-3">Display</h4>

      <div>
        <label for="sleep-timeout" class="form-label">
          Sleep Timeout (seconds)
        </label>
        <input
          type="number"
          class="form-control"
          id="sleep-timeout"
          min="0"
          v-model.number="state.settings.display.sleepTimeout"
        />
      </div>
    </div>

    <div class="mb-2">
      <h4 class="mb-3">Touch</h4>

      <input
        type="checkbox"
        class="form-check-input"
        id="force-calibration"
        v-model="state.settings.touch.forceCalibration"
      />
      <label class="form-check-label ps-2" for="force-calibration"
        >Force calibration</label
      >
    </div>

    <div class="d-grid">
      <button type="submit" data-bs-theme="dark" class="btn btn-secondary">
        Save
      </button>
    </div>
  </form>
</template>

<script lang="ts" setup>
import jsYaml from "js-yaml";
import { reactive, ref } from "vue";
import { fetchConfig, updateConfig, type Config } from "../api/esp32";

const state = reactive<{ settings: Config }>({
  settings: {
    wifi: {
      ssid: undefined,
      password: undefined,
    },
    mqtt: {
      server: undefined,
      port: undefined,
      username: undefined,
      password: undefined,
    },
    security: {
      pin: {
        key: undefined,
        hash: undefined,
      },
      maxNumberOfWrongUnlockAttempts: 3,
    },
    display: {
      sleepTimeout: 5,
    },
    touch: {
      forceCalibration: false,
    },
  },
});

const ensureNumber = () => {
  state.settings.mqtt.port = state.settings.mqtt.port.replace(/\D/g, "");
};

let toastInstance: bootstrap.Toast | null = null;
const toastMessage = ref<string>("");
const toastClass = ref<string>("");
async function onSubmit(event: FormSubmitEvent<Schema>) {
  try {
    const updated = await updateConfig({
      version: "0.0.0",
      wifi: {
        ssid: state.settings.wifi.ssid,
        password: state.settings.wifi.password,
      },
      mqtt: {
        server: state.settings.mqtt.server,
        port: state.settings.mqtt.port || 1883,
        username: state.settings.mqtt.username || "",
        password: state.settings.mqtt.password || "",
      },
      security: {
        pin: {
          hash: state.settings.security.pin.hash || "",
          key: state.settings.security.pin.key || "",
        },
        maxNumberOfWrongUnlockAttempts:
          state.settings.security.maxNumberOfWrongUnlockAttempts || 3,
      },
      display: {
        sleepTimeout: state.settings.display.sleepTimeout || "",
      },
      touch: {
        forceCalibration: state.settings.touch.forceCalibration ? 1 : 0,
      },
    });

    toastMessage.value = "Settings updated successfully!";
    toastClass.value = "bg-dark text-white";
    if (toastInstance) toastInstance.show();
  } catch (error) {
    toastMessage.value = "Error updating settings. Please, try again.";
    toastClass.value = "bg-danger text-white";
    if (toastInstance) toastInstance.show();
  }
}

onMounted(async () => {
  state.settings = await fetchConfig();
  // TODO: move toast to layout
  const toastElement = document.getElementById("submit-toast");
  if (toastElement) {
    toastInstance = new bootstrap.Toast(toastElement, {
      autohide: false,
    });
  }
});
</script>

<style scoped>
.toast {
  position: fixed;
  top: 20px;
  right: 20px;
  z-index: 1000;
}
</style>
