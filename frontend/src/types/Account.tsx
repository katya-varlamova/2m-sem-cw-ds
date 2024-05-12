export interface Account {
    login:	string,
    password?:	string,
    role?: string
}

export interface AuthRequest {
    username: string,
	password: string,
}
export interface CallbackRequest {
    auth_code: number,
}
export interface AuthRequestAdded {
    username: string,
	password: string,
    client_id : number,
    scope_string : string,
}

export interface NewUserRequest {
    firstName: string,
    lastName: string,
    email: string,
    login: string,
    mobilePhone: string,
    password: string,
    role: string
}
