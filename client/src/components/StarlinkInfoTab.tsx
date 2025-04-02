import { zodResolver } from '@hookform/resolvers/zod';
import { useForm } from 'react-hook-form';

import {
	StarlinkInfoData,
	starlinkInfoSchema,
} from '@/schemas/starlinkInfoSchema';
import { useStarlinkInfo } from '@/mutations/useStarlinkInfo';
import { Card, CardContent, CardHeader, CardTitle } from './ui/card';
import { TabsContent } from './ui/tabs';
import { Form, FormControl, FormField, FormItem, FormMessage } from './ui/form';
import { Input } from './ui/input';
import { Button } from './ui/button';

export const StarlinkInfoTab = () => {
	const form = useForm({
		resolver: zodResolver(starlinkInfoSchema),
		defaultValues: { starlinkId: '' },
	});

	const { data, mutateAsync: getStarlinkInfo, isPending } = useStarlinkInfo();

	const handleSubmit = async (data: StarlinkInfoData) => {
		await getStarlinkInfo(data.starlinkId);
	};

	return (
		<TabsContent value='info' className='space-y-8'>
			<Card>
				<CardContent>
					<Form {...form}>
						<form onSubmit={form.handleSubmit(handleSubmit)}>
							<FormField
								control={form.control}
								name='starlinkId'
								render={({ field }) => (
									<FormItem>
										<FormControl>
											<Input {...field} placeholder='Enter ID, f.e. 123' />
										</FormControl>
										<FormMessage />
									</FormItem>
								)}
							/>
							<Button
								disabled={isPending || !form.formState.isValid}
								type='submit'
								className='mt-2'
							>
								Get info about starlink
							</Button>
						</form>
					</Form>
				</CardContent>
			</Card>
			{data && (
				<Card>
					<CardHeader>
						<CardTitle>
							Info about starlink with ID: {form.getValues().starlinkId}
						</CardTitle>
					</CardHeader>
					<CardContent className='grid grid-cols-2 gap-4'>
						<div className='flex items-center gap-4'>
							<span>ID:</span>
							<span>{data.id}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>TLE:</span>
							<span>{data.tle}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Position X:</span>
							<span>{data.positionX}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Position Y:</span>
							<span>{data.positionY}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Position Z:</span>
							<span>{data.positionZ}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Speed X:</span>
							<span>{data.speedX}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Speed Y:</span>
							<span>{data.speedY}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Speed Z:</span>
							<span>{data.speedZ}</span>
						</div>
						<div className='flex items-center gap-4'>
							<span>Country:</span>
							<span>{data.country}</span>
						</div>
					</CardContent>
				</Card>
			)}
		</TabsContent>
	);
};
