package pt.ulisboa.tecnico.distsys.ttt.server;

import io.grpc.stub.StreamObserver;
import pt.ulisboa.tecnico.distsys.ttt.*;

public class TTTServiceImpl extends TTTGrpc.TTTImplBase {

	/** Game implementation. */
	private TTTGame ttt = new TTTGame();

	@Override
	public void currentBoard(CurrentBoardRequest request, StreamObserver<CurrentBoardResponse> responseObserver) {
		CurrentBoardResponse response = CurrentBoardResponse.newBuilder().setBoard(ttt.toString()).build();
		responseObserver.onNext(response);
		responseObserver.onCompleted();
	}

	@Override
	public void play(PlayRequest request, StreamObserver<PlayResponse> responseObserver) {
		PlayResponse response = PlayResponse.newBuilder().setPlayResult(ttt.play(request.getRow(), request.getColumn(), request.getPlayer())).build();
		responseObserver.onNext(response);
		responseObserver.onCompleted();
	}

	@Override
	public void checkWinner(CheckWinnerRequest request, StreamObserver<CheckWinnerResponse> responseObserver) {
		CheckWinnerResponse response = CheckWinnerResponse.newBuilder().setWinner(ttt.checkWinner()).build();
		responseObserver.onNext(response);
		responseObserver.onCompleted();
	}

	@Override
	public void aoCalhas(aoCalhasRequest request, StreamObserver<aoCalhasResponse> responseObserver) {
		aoCalhasResponse response = aoCalhasResponse.newBuilder().setPlayRes(ttt.aoCalhas(request.getPlayer())).build();
		responseObserver.onNext(response);
		responseObserver.onCompleted();
	}

}
