import axiosBackend from "..";
import axios from "axios";

import { AuthRequest, AuthRequestAdded, CallbackRequest } from "types/Account";

interface resp1 {
  status: number, 
  auth_code: number
}

interface resp {
  status: number
}
const IdPUrl = "http://localhost:8090/api/v1";

export const Login = async function (data: AuthRequest): Promise<resp1> {  
   var acc: AuthRequestAdded = {
    username: data.username,
    password: data.password,
    client_id: Number(localStorage.getItem("client_id")),
    scope_string: String(localStorage.getItem("scope_string"))
};
console.log(acc);
  const response = await axios.create({baseURL: IdPUrl})
    .post(`/authcode`, acc)
    .then((data) => data)
    .catch((error) => {
      return { status: error.response?.status, data: error.response.data };
    });
    console.log(response);
  return {
    status: response?.status,
    auth_code: response?.data.auth_code
  };
};

export const Callback = async function (auth_code: number): Promise<resp1> {  
  var req: CallbackRequest = {
    auth_code: auth_code
  };
  console.log(req);
 const response = await axiosBackend
   .post(`/callback`, req)
   .then((data) => data)
   .catch((error) => {
     return { status: error.response?.status, data: error.response.data };
   });
   console.log(response);
   if (response.status === 200) {
    localStorage.setItem("role", response.data.role);
    localStorage.setItem("authToken", response.data.access_token);
  }
 return {
   status: response?.status,
   auth_code: response?.data.auth_code
 };
};

export const Auth = async function (): Promise<resp> {  
  const response = await axiosBackend
    .post(`/authorize`)
    .then((data) => data)
    .catch((error) => {
      return { status: error.response?.status, data: error.response.data };
    });

  if (response.status === 200) {
    localStorage.setItem("client_id", response.data.client_id);
    localStorage.setItem("scope_string", response.data.scope_string);
  }
  return {
    status: response?.status
  };
};
