import { version } from "vue";
import { object, string, number, boolean, type InferType } from "yup";

const CONFIG_SCHEMA = object({
  wifi: object({
    ssid: string().required("SSID is required"),
    password: string().required("WiFi password is required"),
  }),
  authentication: object({
    pin: object({
      pin: string().nullable().notRequired(),
      hash: string().nullable().notRequired(),
    }),
    unlock_attempts: number()
      .default(3)
      .typeError("Max number of wrong unlock attempts must be a number")
      .positive("Max number of wrong unlock attempts must be positive")
      .integer("Max number of wrong unlock attempts must be an integer")
      .strict(),
  }),
  display: object({
    sleep_timeout: number().nullable().notRequired(),
  }),
  touch: object({
    calibrate: boolean().default(false),
  }),
});

type Schema = InferType<typeof CONFIG_SCHEMA>;

async function updateConfig(config: Record<any, any>): Promise<void> {
  try {
    console.debug("Validating configuration");
    await CONFIG_SCHEMA.validate(config, { abortEarly: false });

    console.debug("Updating configuration");
    const response = await fetch("/api/v1/config", {
      method: "PUT",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(config),
    });

    if (!response.ok) {
      throw new Error("Failed to update configuration");
    }

    console.debug("Configuration updated");
    await response.json();
  } catch (error) {
    console.error("Error updating configuration:", error);
    throw error;
  }
}

interface Config {
  wifi: {
    ssid: String;
    password: String;
  };
  authentication: {
    pin: {
      hash: String;
      key: String;
    };
    unlock_attempts: Number;
  };
  display: {
    sleep_timeout: Number;
  };
  touch: {
    calibrate: Boolean;
  };
}

function parse(data: Record<any, any>): Config {
  const config: Config = {
    wifi: {
      ssid: data.wifi.ssid,
      password: data.wifi.password,
    },
    authentication: {
      pin: {
        hash: data.authentication.pin.hash,
        key: data.authentication.pin.key,
      },
      unlock_attempts: data.authentication.unlock_attempts,
    },
    display: {
      sleep_timeout: data.display.sleep_timeout,
    },
    touch: {
      calibrate: data.touch.force_calibration,
    },
  };
  return config;
}

async function fetchConfig(): Promise<Config> {
  try {
    console.debug("Fetching Configuration");
    const response = await fetch("/api/v1/config");
    if (!response.ok) {
      throw new Error("Failed to fetch configuration");
    }

    console.debug("Configuration fetched");
    const data = await response.json();

    console.debug("Parsing configuration");
    const config = parse(data);
    return config;
  } catch (error) {
    console.error("Error fetching configuration:", error);
    throw error;
  }
}

export { updateConfig, fetchConfig, Config };
