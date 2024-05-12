import axiosBackend from "..";
import { NewUserRequest } from "types/Account";
import axios from "axios";
interface resp {
    status: number
}

const IdPUrl = "http://localhost:8090/api/v1";

export const Create = async function(data: NewUserRequest): Promise<resp> {
    let back = axios.create({baseURL: IdPUrl});
    back.interceptors.request.use(function (config) {
        const token = localStorage.getItem("authToken");
        if (config.headers && token) {
            config.headers.Authorization = 'Bearer ' + token;
        }

        return config;
    });
    const response = await back.post(`/register`, data)
        .catch((error) => {
            return {
                status: error.response?.status,
            };
        });

    return {
        status: response?.status,
    };
}
