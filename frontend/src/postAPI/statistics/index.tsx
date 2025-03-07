import { RequestStat } from "types/Statistics";
import axiosBackend from "..";
import { log } from "console";

function formatStartDateToRFC3339(date: Date) {
    const year = date.getUTCFullYear();
    const month = String(date.getUTCMonth() + 1).padStart(2, '0');
    const day = String(date.getUTCDate()).padStart(2, '0');
    const hours = String(date.getUTCHours()).padStart(2, '0');
    const minutes = String(date.getUTCMinutes()).padStart(2, '0');
    const seconds = '00';

    return `${year}-${month}-${day}T${hours}:${minutes}:${seconds}Z`;
}

interface resp {
    status: number
    requests: Array<RequestStat>
}

const ListRequests = async function (beginTime: Date, endTime: Date): Promise<resp> {
    let strBegin =  beginTime.getTime(); //formatStartDateToRFC3339(beginTime)
    let strEnd =  endTime.getTime(); //formatStartDateToRFC3339(endTime)
    console.log(strBegin);
    console.log(strEnd);
    const response = await axiosBackend.get(`statistics?begin_time=${strBegin}&end_time=${strEnd}`);
    console.log(response);
    const requestStats: RequestStat[] = response.data.items.map((responseData) => {
        return {
            path: responseData.path,
            responseCode: responseData.responseCode,
            method: responseData.method,
            startedAt: new Date(Number(responseData.startedAt)),
            finishedAt: new Date(Number(responseData.finishedAt)),
            duration: Number(responseData.duration),
            userName: responseData.userName
        };
    });
    requestStats.sort((a, b) => b.startedAt.getTime() - a.startedAt.getTime());

    return {
        status: response.status,
        requests: requestStats,
    };
}
export default ListRequests;
