const express = require('express');
const cors = require('cors');
const {Pool} = require('pg');

const pool = new Pool ({
    user: 'postgres',
    host: 'localhost',
    database: 'prueba',
    password: '12345',
    port: 5432,
});

const app = express();

app.use(cors());

app.get('/data', async (req, res)=> {
    const result = await pool.query('SELECT * FROM users WHERE id=1');
    res.json(result.rows);
});

app.listen(3306, () => console.log('API running in: http://localhost:3306'));

