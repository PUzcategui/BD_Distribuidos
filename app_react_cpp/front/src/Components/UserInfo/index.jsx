import React from "react";

const UserInfo = ({ userData }) => {

    return (
        <div className="user-info-container">
            <div className="user-info">
                <p className="user-info-id">
                    {userData.id}
                </p>
                <p className="user-info-name">
                    {userData.name}
                </p>
                <p className="user-info-department">
                    {userData.department}
                </p>
                <p className="user-info-location">
                    {userData.location}
                </p>
            </div>
            <div className="user-image">
                <img src={userData.image} alt="user" />
            </div>
        </div>
    );
}   

export default UserInfo;