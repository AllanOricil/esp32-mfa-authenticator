import { version } from "vue";
import { object, string, number, boolean, type InferType } from "yup";

const CONFIG_SCHEMA = object({
  wifi: object({
    ssid: string().required("SSID is required"),
    password: string().required("WiFi password is required"),
  }),
  security: object({
    pin: object({
      pin: string().nullable().notRequired(),
      hash: string().nullable().notRequired(),
    }),
    maxNumberOfWrongUnlockAttempts: number()
      .default(3)
      .typeError("Max number of wrong unlock attempts must be a number")
      .positive("Max number of wrong unlock attempts must be positive")
      .integer("Max number of wrong unlock attempts must be an integer")
      .strict(),
  }),
  display: object({
    sleepTimeout: number().nullable().notRequired(),
  }),
  touch: object({
    forceCalibration: boolean().default(false),
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
  version: String;
  wifi: {
    ssid: String;
    password: String;
  };
  security: {
    pin: {
      hash: String;
      key: String;
    };
    maxNumberOfWrongUnlockAttempts: Number;
  };
  display: {
    sleepTimeout: Number;
  };
  touch: {
    forceCalibration: Boolean;
  };
}

function parse(data: Record<any, any>): Config {
  const config: Config = {
    version: data.version,
    wifi: {
      ssid: data.wifi.ssid,
      password: data.wifi.password,
    },
    security: {
      pin: {
        hash: data.security.pin.hash,
        key: data.security.pin.key,
      },
      maxNumberOfWrongUnlockAttempts:
        data.security.max_number_of_wrong_unlock_attempts,
    },
    display: {
      sleepTimeout: data.display.sleep_timeout,
    },
    touch: {
      forceCalibration: data.touch.force_calibration,
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
