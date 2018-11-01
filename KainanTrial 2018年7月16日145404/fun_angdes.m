function [ ang_mat ] = fun_angdes(Pitch, Yaw, Roll, pitch_var, yaw_var, roll_var)
% layout the angle values in descending order of variances.
%  ang_mat is the output of Eulerian angles in descending order of
%  variances.
    
    col = 3;
    row = length(Pitch);
    ang_mat = zeros(row, col);
	ang_var = [pitch_var yaw_var roll_var];
   
    if max(ang_var) == pitch_var
        if yaw_var > roll_var
			ang_mat = [Pitch Yaw Roll];
		else
			ang_mat = [Pitch Roll Yaw];
        end
    end
	
	if max(ang_var) == yaw_var
        if pitch_var > roll_var
			ang_mat = [Yaw Pitch Roll];
		else
			ang_mat = [Yaw Roll Pitch];
        end
    end
	
	if max(ang_var) == roll_var
        if pitch_var > yaw_var
			ang_mat = [Roll Pitch Yaw];
		else
			ang_mat = [Roll Yaw Pitch];
        end
    end

end

