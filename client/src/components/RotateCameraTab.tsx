import { zodResolver } from '@hookform/resolvers/zod';
import { useForm } from 'react-hook-form';

import {
	RotateCameraData,
	rotateCameraSchema,
} from '@/schemas/rotateCameraSchema';
import { useRotateCamera } from '@/mutations/useRotateCamera';
import { Card, CardContent } from './ui/card';
import { TabsContent } from './ui/tabs';
import { Form, FormControl, FormField, FormItem, FormMessage } from './ui/form';
import { Input } from './ui/input';
import { Button } from './ui/button';

export const RotateCameraTab = () => {
	const form = useForm({
		resolver: zodResolver(rotateCameraSchema),
		defaultValues: {
			angleX: '',
			angleY: '',
			angleZ: '',
		},
	});

	const { mutateAsync: showTrajectory, isPending } = useRotateCamera();

	const handleSubmit = async (data: RotateCameraData) => {
		const { angleX, angleY, angleZ } = data;
		await showTrajectory({
			angleX: parseFloat(angleX),
			angleY: parseFloat(angleY),
			angleZ: parseFloat(angleZ),
		});
	};

	return (
		<TabsContent value='rotate'>
			<Card>
				<CardContent>
					<Form {...form}>
						<form
							className='space-y-4'
							onSubmit={form.handleSubmit(handleSubmit)}
						>
							<FormField
								control={form.control}
								name='angleX'
								render={({ field }) => (
									<FormItem>
										<FormControl>
											<Input
												{...field}
												placeholder='Enter the rotation angle around the X axis'
												onChange={(e) =>
													field.onChange(e.target.value.replace(/[^0-9]/g, ''))
												}
											/>
										</FormControl>
										<FormMessage />
									</FormItem>
								)}
							/>
							<FormField
								control={form.control}
								name='angleY'
								render={({ field }) => (
									<FormItem>
										<FormControl>
											<Input
												{...field}
												placeholder='Enter the rotation angle around the Y axis'
												onChange={(e) =>
													field.onChange(e.target.value.replace(/[^0-9]/g, ''))
												}
											/>
										</FormControl>
										<FormMessage />
									</FormItem>
								)}
							/>
							<FormField
								control={form.control}
								name='angleZ'
								render={({ field }) => (
									<FormItem>
										<FormControl>
											<Input
												{...field}
												placeholder='Enter the rotation angle around the Z axis'
												onChange={(e) =>
													field.onChange(e.target.value.replace(/[^0-9]/g, ''))
												}
											/>
										</FormControl>
										<FormMessage />
									</FormItem>
								)}
							/>
							<Button disabled={isPending} type='submit' className='mt-2'>
								Rotate Camera
							</Button>
						</form>
					</Form>
				</CardContent>
			</Card>
		</TabsContent>
	);
};
