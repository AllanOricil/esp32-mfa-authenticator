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
      <h4 class="mb-3">Authentication</h4>
      <label for="unlock-attempts" class="form-label">Unlock Attempts</label>
      <input
        type="number"
        class="form-control"
        id="unlock-attempts"
        min="1"
        v-model.number="state.settings.authentication.unlock_attempts"
      />
      <div>
        <label for="pin-hash" class="form-label">Pin Hash</label>
        <input
          type="password"
          class="form-control"
          id="pin-hash"
          v-model="state.settings.authentication.pin.hash"
        />
      </div>
      <div>
        <label for="pin-key" class="form-label">Pin Key</label>
        <input
          type="password"
          class="form-control"
          id="pin-key"
          v-model="state.settings.authentication.pin.key"
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
          v-model.number="state.settings.display.sleep_timeout"
        />
      </div>
    </div>

    <div class="mb-2">
      <h4 class="mb-3">Touch</h4>

      <input
        type="checkbox"
        class="form-check-input"
        id="touch-calibrate"
        v-model="state.settings.touch.calibrate"
      />
      <label class="form-check-label ps-2" for="touch-calibrate"
        >Calibrate</label
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
import ESP32MFAAuthenticatorClient from "~/api/esp32-mfa-authenticator-client";

const state = reactive<{ settings: Config }>({
  settings: {
    wifi: {
      ssid: undefined,
      password: undefined,
    },
    authentication: {
      pin: {
        key: undefined,
        hash: undefined,
      },
      unlock_attempts: 3,
    },
    display: {
      sleep_timeout: 5,
    },
    touch: {
      calibrate: false,
    },
  },
});

let toastInstance: bootstrap.Toast | null = null;
const toastMessage = ref<string>("");
const toastClass = ref<string>("");
async function onSubmit(event: FormSubmitEvent<Schema>) {
  try {
    const client = new ESP32MFAAuthenticatorClient();
    const updated = await client.updateConfig({
      wifi: {
        ssid: state.settings.wifi.ssid,
        password: state.settings.wifi.password,
      },
      authentication: {
        pin: {
          hash: state.settings.authentication.pin.hash || "",
          key: state.settings.authentication.pin.key || "",
        },
        unlock_attempts: state.settings.authentication.unlock_attempts || 3,
      },
      display: {
        sleep_timeout: state.settings.display.sleep_timeout || "",
      },
      touch: {
        calibrate: state.settings.touch.calibrate ? 1 : 0,
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
  const client = new ESP32MFAAuthenticatorClient();
  state.settings = await client.fetchConfig();
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
