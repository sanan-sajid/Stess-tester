import React, { useState, useRef, useEffect } from 'react';

const NotificationDropdown = ({ notifications }) => {
  const [open, setOpen] = useState(false);
  const dropdownRef = useRef();

  const toggleDropdown = () => setOpen(!open);

  useEffect(() => {
    const handleClickOutside = (event) => {
      if (dropdownRef.current && !dropdownRef.current.contains(event.target)) {
        setOpen(false);
      }
    };
    document.addEventListener('mousedown', handleClickOutside);
    return () => document.removeEventListener('mousedown', handleClickOutside);
  }, []);

  return (
    <div className="relative inline-block" ref={dropdownRef}>
      <button
        onClick={toggleDropdown}
        className="px-4 py-2 text-sm font-medium text-gray-700 bg-gray-100 border border-gray-300 rounded hover:bg-gray-200 transition"
      >
        Notifications ({notifications.length})
      </button>

      {open && (
        <div className="absolute right-0 mt-2 w-64 bg-white border border-gray-200 rounded shadow-lg z-50 max-h-64 overflow-y-auto">
          {notifications.length === 0 ? (
            <div className="px-4 py-3 text-sm text-gray-500">No notifications</div>
          ) : (
            notifications.map((note, index) => (
              <div
                key={index}
                className="px-4 py-3 text-sm text-gray-800 border-b last:border-b-0 hover:bg-gray-50 transition"
              >
                {note}
              </div>
            ))
          )}
        </div>
      )}
    </div>
  );
};

export default NotificationDropdown;