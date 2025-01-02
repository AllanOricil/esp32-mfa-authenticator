import { object, string, number, boolean, type InferType } from "yup";
import { type AxiosInstance } from "axios";
import { createAxiosInstance, type RetryConfig } from "@/utils/axios";

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

export default class ESP32MFAAuthenticatorClient {
  private client: AxiosInstance;

  constructor(baseURL?: string) {
    this.client = createAxiosInstance(
      baseURL || "/api/v1",
      {
        "Content-Type": "application/json",
      },
      5000,
      5
    );
  }

  public async login(username: string, password: string) {
    try {
      const response = await this.client.post("/auth/login", {
        username,
        password,
      });
      return response.data;
    } catch (error) {
      console.error("Error while loging in: ", error);
      throw error;
    }
  }

  public async logout() {
    try {
      const response = await this.client.post("/auth/logout");
      return response.data;
    } catch (error) {
      console.error("Error while loging out: ", error);
      throw error;
    }
  }

  public async validateSession() {
    try {
      const response = await this.client.post("/auth/validate");
      return response.data;
    } catch (error) {
      console.error("Error while validating session: ", error);
      throw error;
    }
  }

  public async updateConfig(config: Record<any, any>): Promise<void> {
    try {
      console.debug("Validating configuration");
      await CONFIG_SCHEMA.validate(config, { abortEarly: false });
      console.debug("Updating configuration");
      const response = await this.client.put("/config", { config });
      return response.data;
    } catch (error) {
      console.error("Error updating configuration:", error);
      throw error;
    }
  }

  public async fetchConfig(): Promise<Config> {
    try {
      console.debug("Fetching Configuration");
      const response = await this.client.get("/config");
      return response.data;
    } catch (error) {
      console.error("Error fetching configuration:", error);
      throw error;
    }
  }
}
