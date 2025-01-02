import { object, array, string, number, boolean, type InferType } from "yup";
import { type AxiosInstance } from "axios";
import { createAxiosInstance, type RetryConfig } from "@/utils/axios";

const CONFIG_PUT_SCHEMA = object({
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
      .typeError("Unlock attempts must be a number")
      .positive("Unlock attempts must be positive")
      .integer("Unlock attempts must be an integer")
      .strict(),
  }),
  display: object({
    sleep_timeout: number().nullable().notRequired(),
  }),
  touch: object({
    calibrate: boolean().default(false),
  }),
});

const SERVICES_PUT_SCHEMA = array().of(
  object({
    name: string().required("Name is required."),
    group: number()
      .typeError("Group must be a number")
      .integer("Group must be an integer.")
      .positive("Group must be positive")
      .required("Group is required.")
      .strict(),
  })
);

export interface Config {
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

export interface Service {
  name: string;
  totp: string;
  group: number;
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

  public async updateConfig(config: Config): Promise<Config> {
    try {
      console.debug("Validating configuration");
      await CONFIG_PUT_SCHEMA.validate(config, { abortEarly: false });
      console.debug("Updating configuration");
      const response = await this.client.put("/config", { config });
      return response.data as Config;
    } catch (error) {
      console.error("Error while updating configuration:", error);
      throw error;
    }
  }

  public async fetchConfig(): Promise<Config> {
    try {
      console.debug("Fetching Configuration");
      const response = await this.client.get("/config");
      return response.data as Config;
    } catch (error) {
      console.error("Error while fetching configuration:", error);
      throw error;
    }
  }

  public async fetchServices(): Promise<Service[]> {
    try {
      console.debug("Fetching Services");
      const response = await this.client.get("/services");
      return response.data as Service[];
    } catch (error) {
      console.error("Error while fetching services:", error);
      throw error;
    }
  }

  public async updateServices(services: Service[]): Promise<Service[]> {
    try {
      console.debug("Validating services");
      await SERVICES_PUT_SCHEMA.validate(services, { abortEarly: false });
      console.debug("Updating services");
      const response = await this.client.post("/services");
      return response.data as Service[];
    } catch (error) {
      console.error("Error while updating services:", error);
      throw error;
    }
  }
}
