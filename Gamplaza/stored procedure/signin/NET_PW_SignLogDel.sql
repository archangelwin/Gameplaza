USE [QPTreasureDB]
GO

/****** Object:  StoredProcedure [dbo].[NET_PW_SignLogDel]    Script Date: 04/03/2015 17:30:20 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO




CREATE PROC [dbo].[NET_PW_SignLogDel]
AS
BEGIN
	DECLARE @Type INT,@UserID INT,@SignDate DATETIME
	--@Type签到类型（0按次数，1按天数，2补签）
	SELECT @Type=[Type] FROM QPTreasureDB.dbo.SignDay
	
	IF(@Type=0)--按次数
		BEGIN
			DECLARE PumpRecordd1 CURSOR FOR 
			SELECT DISTINCT UserID FROM QPTreasureDB.dbo.SignLog WHERE Continuous=7
			OPEN PumpRecordd1
			FETCH NEXT FROM PumpRecordd1 INTO @UserID
			WHILE @@FETCH_STATUS=0
			  BEGIN
				DELETE FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID
				FETCH NEXT FROM PumpRecordd1 INTO @UserID
			  END
			CLOSE PumpRecordd1;
			DEALLOCATE PumpRecordd1;
		END
	ELSE IF(@Type=1)  --按天数
		BEGIN
			--删除签满7天的数据
			DECLARE PumpRecordd2 CURSOR FOR 
			SELECT DISTINCT UserID FROM QPTreasureDB.dbo.SignLog WHERE Continuous=7
			OPEN PumpRecordd2
			FETCH NEXT FROM PumpRecordd2 INTO @UserID
			WHILE @@FETCH_STATUS=0
			  BEGIN
				DELETE FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID
				FETCH NEXT FROM PumpRecordd2 INTO @UserID
			  END
			CLOSE PumpRecordd2;
			DEALLOCATE PumpRecordd2;
			
			--删除有间隔日期的数据
			DECLARE PumpRecordd3 CURSOR FOR
			SELECT DISTINCT UserID FROM QPTreasureDB.dbo.SignLog
			OPEN PumpRecordd3;
			FETCH NEXT FROM PumpRecordd3 INTO @UserID
			WHILE @@FETCH_STATUS=0
			  BEGIN
				SELECT TOP 1 @SignDate=SignDate FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID ORDER BY SignDate DESC
				IF ISNULL(DATEDIFF(DAY, CONVERT(NVARCHAR(10),@SignDate,120),CONVERT(NVARCHAR(10),GETDATE(),120)),0)>=2
					BEGIN
						DELETE FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID
					END
				FETCH NEXT FROM PumpRecordd3 INTO @UserID
			  END
			CLOSE PumpRecordd3;
			DEALLOCATE PumpRecordd3;
			
		END
	ELSE IF(@Type=2) --含补签
		BEGIN
			
			DECLARE PumpRecordd4 CURSOR FOR 
			SELECT DISTINCT UserID FROM QPTreasureDB.dbo.SignLog WHERE Continuous=7
			OPEN PumpRecordd4
			FETCH NEXT FROM PumpRecordd4 INTO @UserID
			WHILE @@FETCH_STATUS=0
			  BEGIN
				IF(SELECT COUNT(*) FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID AND Continuous IN (1,2,3,4,5,6,7))=7
					BEGIN
						DELETE FROM QPTreasureDB.dbo.SignLog WHERE UserID=@UserID
					END
				FETCH NEXT FROM PumpRecordd4 INTO @UserID
			  END
			CLOSE PumpRecordd4;
			DEALLOCATE PumpRecordd4;
			
		END
END




GO

