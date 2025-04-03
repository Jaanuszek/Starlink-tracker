'use client';

import { Tabs, TabsList, TabsTrigger } from './ui/tabs';
import { DisableStarlinkTab } from './DisableStarlinkTab';
import { EnableStarlinkTab } from './EnableStarlinkTab';
import { LoadStarlinksTab } from './LoadStarlinksTab';
import { HighlighteStarlinkTab } from './HighlightStarlinkTab';
import { ToggleCountriesBorderTab } from './ToggleCountriesBorderTab';
import { ShowTrajectoryTab } from './ShowTrajectoryTab';
import { RotateCameraTab } from './RotateCameraTab';
import { StarlinkInfoTab } from './StarlinkInfoTab';

export default function HomeContent() {
	return (
		<div className='p-4'>
			<Tabs defaultValue='load'>
				<TabsList>
					<TabsTrigger value='load'>Load Starlinks</TabsTrigger>
					<TabsTrigger value='disable'>Disable Starlink</TabsTrigger>
					<TabsTrigger value='enable'>Enable Starlink</TabsTrigger>
					<TabsTrigger value='highlight'>Highlight Startlink</TabsTrigger>
					<TabsTrigger value='border'>Toggle Countries Border</TabsTrigger>
					<TabsTrigger value='trajectory'>Show Starlink Trajectory</TabsTrigger>
					<TabsTrigger value='rotate'>Rotate Camera</TabsTrigger>
					<TabsTrigger value='info'>Starlink Info</TabsTrigger>
				</TabsList>
				<LoadStarlinksTab />
				<DisableStarlinkTab />
				<EnableStarlinkTab />
				<HighlighteStarlinkTab />
				<ToggleCountriesBorderTab />
				<ShowTrajectoryTab />
				<RotateCameraTab />
				<StarlinkInfoTab />
			</Tabs>
		</div>
	);
}
