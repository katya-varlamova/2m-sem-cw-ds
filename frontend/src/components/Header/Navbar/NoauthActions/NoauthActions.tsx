import React from "react";
import { Box, Text, Link } from "@chakra-ui/react";

import styles from "../Navbar.module.scss";

import LoginIcon from 'components/Icons/Login'
import { Auth } from "postAPI/accounts/Login";
import RoundButton from "components/RoundButton";
export interface NoauthActionsProps {}
const NoauthActions: React.FC<NoauthActionsProps> = () => {
//     <Link className={styles['user-act']} href="/auth/signin">
//     <Box><LoginIcon/></Box>
//     <Text>Войти</Text>
// </Link>

    return (
        <RoundButton onClick={ () => {
            Auth().then(data => {
                if (data.status === 200) 
                    window.location.href = '/auth/signin';})
        } }> Войти </RoundButton>
        
    )
}

export default React.memo(NoauthActions);
