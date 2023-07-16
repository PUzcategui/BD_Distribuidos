import UserInfo from "./Components/UserInfo";
import "./App.css";
import React, { useEffect, useState } from 'react'

function App() {

    const [data, setData] = useState(null);

    useEffect( () => { 
        const fetchData = async () => {
            console.log("a2")
            const response = await fetch('http://localhost:3307/data');
            const jsonData = await response.json();
            console.log(jsonData)
            setData(jsonData);
        }

        fetchData();
    }, []);

    
    return (
        < UserInfo userData={data[0]} />  
    )

};

export default App;

