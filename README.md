import React, { useState, useEffect, useRef } from "react";
import axios from "axios";

function Dropdown({ email }) { // Removed onUpdate as it wasn't used
  const [isOpen, setIsOpen] = useState(false);
  const [notifications, setNotifications] = useState([]);
  const [unreadCount, setUnreadCount] = useState(0);
  // const [dropdownState, setDropdownState] = useState(0); // This state seems redundant with isOpen for toggling

  const dropdownRef = useRef(null); // For detecting clicks outside

  // const intervalId = useRef(null); // To store interval ID correctly if it were used

  useEffect(() => {
    if (email) {
      fetchNotifications();
      // Example for SSE (ensure backend supports this specific path and format)
      const sse = new EventSource(
        `http://localhost:8080/getNotification/${email}`
      );
      sse.onmessage = (event) => {
        console.log("SSE message received:", event.data);
        fetchNotifications(); // Refresh notifications on SSE message
      };
      sse.onerror = (err) => {
        console.error("SSE Error:", err);
        // sse.close(); // Optionally close on error or implement retry logic
      };

      return () => {
        sse.close(); // Cleanup SSE connection on component unmount
        // if (intervalId.current) clearInterval(intervalId.current); // Correct cleanup for interval if used
      };
    }
  }, [email]);

  // Click outside handler
  useEffect(() => {
    const handleClickOutside = (event) => {
      if (dropdownRef.current && !dropdownRef.current.contains(event.target)) {
        if (isOpen) { // Only mark as seen if dropdown was open
            markNotificationsAsSeen();
        }
        setIsOpen(false);
      }
    };
    document.addEventListener("mousedown", handleClickOutside);
    return () => {
      document.removeEventListener("mousedown", handleClickOutside);
    };
  }, [isOpen, notifications]); // Add notifications to dependencies of handleClickOutside if markNotificationsAsSeen depends on it

  const fetchNotifications = async () => {
    if (!email) return;
    try {
      const res1 = await axios.post("http://localhost:8080/getStatus", {
        email,
      });
      // Assuming res1.data.data contains the notifications array
      const fetchedNotifications = res1.data.data || [];

      const updatedNotifications = await Promise.all(
        fetchedNotifications.map(async (item) => {
          try {
            const scholarshipResponse = await axios.get(
              `http://localhost:8080/getScholarship/${item.scholarshipId}`
            );
            const scholarship = scholarshipResponse.data.data;
            return {
              ...item,
              name: scholarship.name,
              cgpaRequirement: scholarship.cgpaRequirement,
              ageRequirement: scholarship.ageRequirement,
              incomeRequirement: scholarship.incomeRequirement,
              details: scholarship.details,
            };
          } catch (scholarshipErr) {
            console.error("Error fetching scholarship for notification:", scholarshipErr);
            return { ...item, name: "Unknown Scholarship" }; // Fallback
          }
        })
      );

      setNotifications(updatedNotifications);
      const notSeenCount = updatedNotifications.filter(
        (notif) => notif.status === "not_seen" || notif.status === "accepted_by_admin" || notif.status === "rejected_by_admin"
      ).length; // Example: consider admin actions as "unread" for user until "seen_by_user"
      setUnreadCount(notSeenCount);

    } catch (err) {
      console.error("Error fetching notifications:", err);
      // Do not alert here, as it can be annoying if fetch is frequent
    }
  };

  const toggleDropdown = () => {
    const newIsOpenState = !isOpen;
    setIsOpen(newIsOpenState);
    if (!newIsOpenState && notifications.some(n => n.status === "not_seen" || n.status === "accepted_by_admin" || n.status === "rejected_by_admin")) {
      // If closing and there are unread, mark them as seen.
      markNotificationsAsSeen();
    }
  };

  const markNotificationsAsSeen = async () => {
    // Mark specific types of notifications as "seen_by_user"
    // For this example, let's assume "not_seen", "accepted_by_admin", "rejected_by_admin" become "seen_by_user"
    const unseenNotifications = notifications.filter(
      (notif) => notif.status === "not_seen" || notif.status === "accepted_by_admin" || notif.status === "rejected_by_admin"
    );

    if (unseenNotifications.length === 0) return;

    try {
      for (const notif of unseenNotifications) {
        // IMPORTANT: The backend needs a status like "seen_by_user"
        // For now, I'll use "seen" as per original code, but this might conflict with Admin logic
        // if "seen" means something else in the student's context.
        // A better status might be "acknowledged_by_student" or "viewed_by_student".
        await axios.put(`http://localhost:8080/updateStatus/${notif.id}`, {
          ...notif, // Send all notification data
          status: "seen", // This should ideally be a distinct status like "seen_by_user"
        });
      }
      fetchNotifications(); // Refresh to get updated counts and statuses
    } catch (err) {
      console.error("Error updating notification status to seen:", err);
    }
  };


  const handleStudentAction = async (
    notification, // Pass the whole notification object
    newStatus // "applied" or "rejected_by_student"
  ) => {
    try {
      await axios.put(`http://localhost:8080/updateStatus/${notification.id}`, {
        id: notification.id,
        status: newStatus,
        studentEmail: notification.studentEmail, // ensure this is correct
        scholarshipId: notification.scholarshipId,
        // include other fields from notification if your backend expects them
      });
      alert(`Scholarship application status updated to ${newStatus.replace(/_/g, ' ')}.`);
      fetchNotifications(); // Refresh notifications
    } catch (err) {
      console.error("Error updating student action status:", err);
      alert(err.response?.data?.message || err.message || "An error occurred.");
    }
  };


  // Filter notifications for display in dropdown: only "not_seen", "seen" (or "seen_by_user"), "accepted_by_admin", "rejected_by_admin"
  const displayableNotifications = notifications.filter(notif =>
    ["not_seen", "seen", "accepted_by_admin", "rejected_by_admin"].includes(notif.status)
  ).sort((a, b) => { // Show actionable notifications first
    const order = { "not_seen": 0, "accepted_by_admin": 1, "rejected_by_admin": 2, "seen": 3 };
    return (order[a.status] || 99) - (order[b.status] || 99);
  });


  return (
    <div className="relative" ref={dropdownRef}>
      <button
        className="relative inline-flex items-center px-3 py-2 border border-gray-300 shadow-sm text-sm font-medium rounded-md text-gray-700 bg-white hover:bg-gray-50 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-red-500"
        onClick={toggleDropdown}
        aria-haspopup="true"
        aria-expanded={isOpen}
      >
        <svg xmlns="http://www.w3.org/2000/svg" className="h-5 w-5 text-gray-600" fill="none" viewBox="0 0 24 24" stroke="currentColor">
          <path strokeLinecap="round" strokeLinejoin="round" strokeWidth="2" d="M15 17h5l-1.405-1.405A2.032 2.032 0 0118 14.158V11a6.002 6.002 0 00-4-5.659V5a2 2 0 10-4 0v.341C7.67 6.165 6 8.388 6 11v3.159c0 .538-.214 1.055-.595 1.436L4 17h5m6 0v1a3 3 0 11-6 0v-1m6 0H9" />
        </svg>
        {unreadCount > 0 && (
          <span className="absolute -top-2 -right-2 inline-flex items-center justify-center px-2 py-1 text-xs font-bold leading-none text-red-100 bg-red-600 rounded-full">
            {unreadCount}
          </span>
        )}
      </button>

      {isOpen && (
        <div className="origin-top-right absolute right-0 mt-2 w-96 rounded-md shadow-lg bg-white ring-1 ring-black ring-opacity-5 focus:outline-none z-50">
          <div className="py-1 max-h-96 overflow-y-auto" role="menu" aria-orientation="vertical" aria-labelledby="options-menu">
            {displayableNotifications.length === 0 ? (
              <p className="px-4 py-3 text-sm text-gray-500">No new notifications.</p>
            ) : (
              displayableNotifications.map((notification) => (
                <div key={notification.id} className="block px-4 py-3 text-sm text-gray-700 border-b last:border-b-0 hover:bg-gray-100" role="menuitem">
                  <p className="font-semibold text-gray-800">{notification.name}</p>
                  <p className="text-xs text-gray-500 truncate" title={notification.details}>
                    CGPA: {notification.cgpaRequirement}, Age: {notification.ageRequirement}, Income: {notification.incomeRequirement}
                  </p>
                  <p className="mt-1 text-xs">
                    Status: <span className={`font-medium ${
                        notification.status === "accepted_by_admin" ? "text-green-600" :
                        notification.status === "rejected_by_admin" ? "text-red-600" :
                        notification.status === "not_seen" ? "text-blue-600" : "text-gray-600"
                    }`}>
                        {notification.status.replace(/_/g, ' ')}
                    </span>
                  </p>
                  {/* Student actions: Apply (if eligible and "not_seen" or "seen"), or acknowledge admin decisions */}
                  {(notification.status === "not_seen" || notification.status === "seen") && (
                     // Assuming these are scholarships the student hasn't interacted with yet,
                     // or re-applying is not an option here based on original code.
                     // The original code had Apply/Reject for "not_seen" or "seen" which is a bit ambiguous.
                     // Let's assume "not_seen" means "recommended, take action"
                    <div className="mt-2 space-x-2">
                      <button
                        className="bg-green-500 hover:bg-green-600 text-white font-semibold py-1 px-2 rounded text-xs"
                        onClick={() => handleStudentAction(notification, "applied")}
                      >
                        Apply
                      </button>
                      <button
                        className="bg-red-500 hover:bg-red-600 text-white font-semibold py-1 px-2 rounded text-xs"
                        onClick={() => handleStudentAction(notification, "rejected_by_student")}
                      >
                        Reject
                      </button>
                    </div>
                  )}
                  {(notification.status === "accepted_by_admin" || notification.status === "rejected_by_admin") && (
                    <p className="mt-1 text-xs text-gray-500 italic">Admin has reviewed this application.</p>
                    // Optionally add an "Acknowledge" button if needed for these statuses
                  )}
                </div>
              ))
            )}
          </div>
        </div>
      )}
    </div>
  );
}

export default Dropdown;