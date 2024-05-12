export const navItems = {
    '': [
        { name: "Новый пользователь", ref: "/auth/signup" }
    ],
    'admin': [
        { name: "Новый пользователь", ref: "/auth/signup" },
        { name: "Рейсы", ref: "/" },
        { name: "Личный кабинет", ref: "/tickets" },
        { name: "Статистика", ref: "/statistics" },
    ],
    'user': [
        { name: "Рейсы", ref: "/" },
        { name: "Личный кабинет", ref: "/tickets" },
    ],
}
