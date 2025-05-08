Here’s a simple React code snippet that displays an array of objects in a table and refreshes the data when a button is pressed:

Example: React Table with Refresh Button

import React, { useState, useEffect } from 'react';

const fetchData = async () => {
  // Simulating a fetch call. Replace this with actual API call.
  return [
    { id: 1, name: 'John', age: 28 },
    { id: 2, name: 'Alice', age: 25 },
    { id: 3, name: 'Bob', age: 32 },
  ];
};

const DataTable = () => {
  const [data, setData] = useState([]);

  const loadData = async () => {
    const newData = await fetchData();
    setData(newData);
  };

  useEffect(() => {
    loadData(); // Load data on component mount
  }, []);

  return (
    <div>
      <button onClick={loadData}>Refresh</button>
      <table border="1" cellPadding="5">
        <thead>
          <tr>
            <th>ID</th>
            <th>Name</th>
            <th>Age</th>
          </tr>
        </thead>
        <tbody>
          {data.map((obj) => (
            <tr key={obj.id}>
              <td>{obj.id}</td>
              <td>{obj.name}</td>
              <td>{obj.age}</td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};

export default DataTable;

Notes:
	•	Replace the fetchData() function with your actual data fetching logic (like calling an API).
	•	The table updates every time the “Refresh” button is clicked.

Would you like a version that includes loading states or error handling?