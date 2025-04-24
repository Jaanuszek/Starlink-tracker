'use client';

import { Tabs, TabsList, TabsTrigger } from './ui/tabs';
import { LoadStarlinksTab } from './LoadStarlinksTab';
import { HighlighteStarlinkTab } from './HighlightStarlinkTab';
import { ToggleCountriesBorderTab } from './ToggleCountriesBorderTab';
import { RotateCameraTab } from './RotateCameraTab';
import { StarlinkInfoTab } from './StarlinkInfoTab';
import { ToggleVisibilityTab } from './ToggleVisibilityTab';
import { ToggleTrajectoryVisibilityTab } from './ToggleTrajectoryVisibilityTab';

export default function HomeContent() {
	return (
		<div className='p-4'>
			<Tabs defaultValue='load'>
				<TabsList>
					<TabsTrigger value='load'>Load Starlinks</TabsTrigger>
					<TabsTrigger value='toggleVisibility'>Toggle visibility</TabsTrigger>
					<TabsTrigger value='toggleTrajectoryVisibility'>
						Toggle Trajectory Visibility
					</TabsTrigger>
					<TabsTrigger value='highlight'>Highlight Startlink</TabsTrigger>
					<TabsTrigger value='border'>Toggle Countries Border</TabsTrigger>
					<TabsTrigger value='rotate'>Rotate Camera</TabsTrigger>
					<TabsTrigger value='info'>Starlink Info</TabsTrigger>
				</TabsList>
				<LoadStarlinksTab />
				<ToggleVisibilityTab />
				<ToggleTrajectoryVisibilityTab />
				<HighlighteStarlinkTab />
				<ToggleCountriesBorderTab />
				<RotateCameraTab />
				<StarlinkInfoTab />
			</Tabs>
		</div>
	);
}
