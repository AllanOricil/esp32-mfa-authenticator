import axios, {
  type AxiosInstance,
  type AxiosResponse,
  type AxiosError,
  type InternalAxiosRequestConfig,
} from "axios";
import axiosRetry from "axios-retry";
import { v7 as uuidv7 } from "uuid";
import type { SecureLogger } from "./logger";

type AxiosConfigWithMetadata = InternalAxiosRequestConfig & {
  metadata?: {
    axiosId: string;
  };
};

const interceptRequest = (config: AxiosConfigWithMetadata) => {
  const axiosId = uuidv7();

  console.log("outbound request", {
    baseUrl: config.baseURL,
    method: config.method,
    data: config.data,
    params: config.params,
    url: config.url,
    headers: config.headers,
    axiosId,
  });

  config.metadata = { axiosId };

  return config;
};

const interceptSuccessResponse = (response: AxiosResponse) => {
  console.log("outbound response success", {
    baseUrl: response.config.baseURL,
    url: response.config.url,
    status: `${response.status}:${response.statusText}`,
    headers: response.headers,
    body: response.data,
    axiosId: (response.config as AxiosConfigWithMetadata)?.metadata?.axiosId,
  });

  return response;
};

const interceptorErrorResponse = async (error: any) => {
  if (error instanceof Error) {
    const axiosError = error as AxiosError;
    console.log("outbound response failure", {
      baseUrl: axiosError?.response?.config.baseURL,
      url: axiosError?.response?.config.url,
      status: axiosError.response?.status,
      headers: axiosError.response?.headers,
      body: axiosError.response?.data,
      axiosId: (axiosError?.response?.config as AxiosConfigWithMetadata)
        ?.metadata?.axiosId,
    });
  }

  return await Promise.reject(error);
};

interface RetryConfig extends IAxiosRetryConfig {}

const createAxiosInstance = (
  baseURL: string,
  headers?: Record<string, string>,
  timeout?: number,
  maxRedirects?: number
): AxiosInstance => {
  const axiosInstance: AxiosInstance = axios.create({
    baseURL,
    headers,
    timeout,
    maxRedirects,
  });

  axiosInstance.interceptors.request.use(interceptRequest);
  axiosInstance.interceptors.response.use(
    interceptSuccessResponse,
    interceptorErrorResponse
  );

  axiosRetry(axiosInstance, {
    retries: 3,
    retryDelay: axiosRetry.exponentialDelay,
  });

  return axiosInstance;
};

export { createAxiosInstance, RetryConfig };
