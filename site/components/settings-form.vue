<template>
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
          type="text"
          class="form-control"
          id="port"
          @input="ensureNumber"
          v-model.trim="state.settings.mqtt.port"
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
        <label for="sleep-timeout" class="form-label"
          >Sleep Timeout (seconds)</label
        >
        <input
          type="number"
          class="form-control"
          id="sleep-timeout"
          v-model="state.settings.display.sleep_timeout"
        />
      </div>
    </div>

    <div class="mb-2">
      <h4 class="mb-3">Touch</h4>

      <input
        type="checkbox"
        class="form-check-input"
        id="force-calibration"
        v-model="state.settings.touch.force_calibration"
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
import { object, string, number, boolean, type InferType } from "yup";
import jsYaml from "js-yaml";
import { reactive, ref } from "vue";

const errors = ref({});

const schema = object({
  settings: object({
    wifi: object({
      ssid: string().required("SSID is required"),
      password: string().required("WiFi password is required"),
    }),
    mqtt: object({
      server: string().required("MQTT server is required"),
      port: number()
        .default(1883)
        .required("Port is required")
        .typeError("Port number must be a number")
        .positive("Port number must be positive")
        .integer("Port number must be an integer")
        .strict(),
      username: string().nullable().notRequired(),
      password: string().nullable().notRequired(),
    }),
    security: object({
      pin: object({
        pin: string().nullable().notRequired(),
        hash: string().nullable().notRequired(),
      }),
    }),
    display: object({
      sleep_timeout: number().nullable().notRequired(),
    }),
    touch: object({
      force_calibration: boolean().default(false),
    }),
  }),
});

type Schema = InferType<typeof schema>;

const state = reactive({
  settings: {
    wifi: {
      ssid: undefined,
      password: undefined,
    },
    mqtt: {
      server: undefined,
      port: 1883,
      username: undefined,
      password: undefined,
    },
    security: {
      pin: {
        key: undefined,
        hash: undefined,
      },
    },
    display: {
      sleep_timeout: 5,
    },
    touch: {
      force_calibration: false,
    },
  },
});

const ensureNumber = () => {
  state.settings.mqtt.port = state.settings.mqtt.port.replace(/\D/g, "");
};

async function onSubmit(event: FormSubmitEvent<Schema>) {
  // Do something with event.data
  try {
    await schema.validate(state, { abortEarly: false });

    const yamlContent = jsYaml.dump({
      version: "0.0.0",
      wifi: {
        ssid: state.settings.wifi.ssid,
        password: state.settings.wifi.password,
      },
      mqtt: {
        server: state.settings.mqtt.server,
        port: state.settings.mqtt.port,
        username: state.settings.mqtt.username || "",
        password: state.settings.mqtt.password || "",
      },
      security: {
        pin: {
          hash: state.settings.security.pin.hash || "",
          key: state.settings.security.pin.key || "",
        },
      },
      display: {
        sleep_timeout: state.settings.display.sleep_timeout || "",
      },
      touch: {
        force_calibration: state.settings.touch.force_calibration ? 1 : 0,
      },
    });

    // Log YAML content for verification (remove in production)
    console.log(yamlContent);
  } catch (error) {
    const formattedErrors = {};
    error.inner.forEach((e) => {
      const key = e.path;
      formattedErrors[key] = e.message;
    });
    errors.value = formattedErrors;
  }
}
</script>

<style></style>
