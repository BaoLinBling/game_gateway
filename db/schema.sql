-- =========================
-- sessions 表
-- =========================
CREATE TABLE IF NOT EXISTS sessions (
    id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY COMMENT '自增ID',

    user_id BIGINT UNSIGNED NOT NULL COMMENT '用户ID',

    jwt VARCHAR(128) NOT NULL COMMENT '登录token',

    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',

    expires_at TIMESTAMP NOT NULL COMMENT '过期时间',

    INDEX idx_user_id (user_id),
    UNIQUE KEY uk_jwt (jwt)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户登录会话表';
