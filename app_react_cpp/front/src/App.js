import UserInfo from "./Components/UserInfo";
import "./App.css";
import React, { useEffect, useState } from 'react'

function App() {

    const [data, setData] = useState(null);

    useEffect( () => { 
        const fetchData = async () => {
            const response = await fetch('http://localhost:3306/data');
            const jsonData = await response.json();
            setData(jsonData);
        }

        fetchData();
    }, []);

    console.log(data);
    
    return (
        <>
            <UserInfo userData={data[0]} />
        </>
    )

};

export default App;

